#!/usr/bin/env python -Wignore
"""
Conversion utility for turning COLLADA .dae files into EECE478 model files.
Requires pycollada and PIL.
"""

import collada
from __future__ import print_function
from numpy import array, concatenate, empty
import sys
from os import path
import argparse
import Image


class ColladaSucks(object):
    """
    Converts a COLLADA file into lists of textures, vertices, normals, and
    triangles, and can write out a .model file.
    """

    # Allowed sizes for images
    good_sizes = [2**i for i in range(10)]

    def __init__(self, filename, ignore=False):
        # Ignore errors if ignore flag is True
        ignore_exs = []
        if ignore:
            ignore_exs.extend([
                collada.DaeUnsupportedError,
                collada.DaeBrokenRefError,
            ])

        self.name, ext = path.splitext(path.basename(filename))

        # Parse the Collada file
        self.scene = collada.Collada(filename, ignore=ignore_exs)

        if self.scene.errors:
            print(*self.scene.errors, sep='\n', file=sys.stderr)

        self.textures = []
        self._vertices = empty((0, 3))
        self._normals = empty((0, 3))
        self.triangles = []

        for i, triangle in enumerate(self.get_triangles()):

            # Create triangle indices (reversed for OpenGL)
            vertex_ind = array([0, 1, 2]) + len(self._vertices)
            normal = len(self._normals)

            if triangle.texcoords:
                # Invert texture coordinates along y axis
                coords = [(c[0], 1 - c[1]) for c in triangle.texcoords[0]]
                surface, sampler = triangle.material.params

                image_file = self.convert_to_ppm(surface.image.path)

                # Only add the texture file if necessary
                if image_file not in self.textures:
                    texture = len(self.textures)
                    self.textures.append(image_file)
                else:
                    texture = self.textures.index(image_file)
            else:
                print("Warning: no texture for triangle", i, file=sys.stderr)
                # Skip this triangle
                continue

            triangle_info = (
                vertex_ind[0], vertex_ind[1], vertex_ind[2],
                normal,
                texture,
                coords[0][0], coords[0][1],
                coords[1][0], coords[1][1],
                coords[2][0], coords[2][1],
            )

            self.triangles.append(triangle_info)
            self._vertices = concatenate((self._vertices, triangle.vertices))
            self._normals = concatenate((self._normals, [triangle.normals[0]]))


        # Flip x axis for all vertices and normals
        self._vertices[:, 0] *= -1
        self._normals[:, 0] *= -1


    @property
    def vertices(self):
        """ Turns the vertices ndarray into a generator of tuples. """
        for vertex in self._vertices:
            yield tuple(vertex)

    @property
    def normals(self):
        """ Turns the normals ndarray into a generator of tuples. """
        for vertex in self._normals:
            yield tuple(vertex)

    def get_triangles(self):
        """
        A generator that pulls all the triangles out of the Collada scene.
        """
        for geometry in self.scene.scene.objects('geometry'):
            for primitive in geometry.primitives():
                for triangle in primitive.triangles():
                    yield triangle

    def scale(self, factor):
        """ Scales the vertices by the given factor """
        self._vertices = self._vertices * factor

    def set_axes(self, indices):
        """ Rearranges the axes of the vertices and normals """
        t = self._vertices.transpose()
        t2 = t.copy()
        t2[indices[0]], t2[indices[1]], t2[indices[2]] = t
        self._vertices = t2.transpose()

    def write_model(self, outfile):
        """
        Writes a model specification to the given file-like object.
        """
        # Shorthand function for printing a formatted collection to outfile
        line = lambda template, collection: print(
            *(template % item for item in collection), sep='\n', file=outfile
        )

        line('b %s', [self.name])
        line('i %s', self.textures)
        line('v %f %f %f', self.vertices)
        line('n %f %f %f', self.normals)
        line('t %d %d %d %d %d %f %f %f %f %f %f', self.triangles)

    def print_info(self):
        print("Building Name: %s" % self.name, file=sys.stderr)
        print("Textures: %d" % len(self.textures), file=sys.stderr)
        print("Vertices: %d" % len(self._vertices), file=sys.stderr)
        print("Normals: %d" % len(self._normals), file=sys.stderr)
        print("Triangles: %d" % len(self.triangles), file=sys.stderr)


    def convert_to_ppm(self, filename):
        """ 
        Converts an image to PPM and returns the new path 
        """
        prefix, ext = path.splitext(filename)
        newfile = ''.join((prefix, '.ppm'))

        im = Image.open(filename)
        width, height = im.size

        if width not in self.good_sizes or height not in self.good_sizes:
            print('Warning: image size for "%s" is not a power of two' % 
                    filename, file=sys.stderr)

        try:
            im.save(newfile, 'PPM')
        except Exception, e:
            print(e, file=sys.stderr)
            raise Exception("Error converting texture file: %s" % filename)

        return newfile

    def texture_transform(self, coords):
        """
        Transforms texture coordinates by inverting along the y axis.  
        """
        return [(coord[0], 1 - coord[1]) 
                for coord in coords]


def main(argv):
    parser = argparse.ArgumentParser()

    parser.add_argument('input_file')

    parser.add_argument('output_file', nargs='?', type=argparse.FileType('w'),
                        default=sys.stdout)

    parser.add_argument('-i', '--ignore', dest='ignore', action='store_true',
            help='Ignore errors when parsing COLLADA file')

    parser.add_argument('-v', '--verbose', action='store_true',
            help='Print out extra info about model')

    parser.add_argument('-s', '--scale', type=float, default=None,
            help='Scale the vertices away from the origin by this factor')

    args = parser.parse_args()

    cs = ColladaSucks(args.input_file, ignore=args.ignore)

    if args.scale:
        cs.scale(args.scale)

    # Swap Y and Z axes since they apparently mean different things
    cs.set_axes([0, 2, 1])

    if args.verbose:
        cs.print_info()

    cs.write_model(args.output_file)


if __name__ == '__main__':
    main(sys.argv)
