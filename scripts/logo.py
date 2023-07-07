from collections.abc import Sequence
import dataclasses
import io
import random

from absl import app
from absl import flags
import svgwrite

FLAGS = flags.FLAGS

flags.DEFINE_integer("scale", 15, "Size of 'pixels'")
flags.DEFINE_string("color", "#d8ff00", "Color of logo")
flags.DEFINE_string("output", "-", "Ouput file path ('-' for stdout)")
flags.DEFINE_integer("seed", 42, "RNG seed for dissolves")

Drawing = svgwrite.Drawing


@dataclasses.dataclass(frozen=True)
class Point:
    x: int
    y: int

    def __mul__(self, other: int):
        return Point(self.x * other, self.y * other)


def dissolve_left(doc: Drawing, start: Point, width: int, height: int) -> None:
    pixels = [*range(height)]
    for x in range(1, width):
        random.shuffle(pixels)
        count = height * x // width
        clear = pixels[:count]
        for y in clear:
            doc.add(doc.rect((start.x - x - 1, start.y + y), (1, 1), fill="#fff"))


def dissolve_down(doc: Drawing, start: Point, width: int, height: int) -> None:
    pixels = [*range(width)]
    for y in range(1, height):
        random.shuffle(pixels)
        count = width * y // height
        clear = pixels[:count]
        for x in clear:
            doc.add(doc.rect((start.x + x, start.y + y), (1, 1), fill="#fff"))


def main(argv: Sequence[str]) -> None:
    doc = Drawing();

    random.seed(FLAGS.seed)

    rects: list[tuple[int, int, int, int]] = [
            # letter g
            (0, 1, 1, 1),
            (1, 0, 3, 1),
            (1, 2, 3, 1),
            (1, 4, 3, 1),
            (4, 1, 1, 3),
            # letter a
            (6, 3, 1, 1),
            (7, 0, 3, 1),
            (7, 2, 3, 1),
            (7, 4, 3, 1),
            (10, 1, 1, 4),
            # letter m
            (12, 0, 1, 5),
            (13, 0, 1, 1),
            (14, 1, 1, 4),
            (15, 0, 1, 1),
            (16, 1, 1, 4),
            ]

    for r in rects:
        doc.add(
                doc.rect(
                    (r[0] * FLAGS.scale, r[1] * FLAGS.scale),
                    (r[2] * FLAGS.scale, r[3] * FLAGS.scale),
                    fill=FLAGS.color,
                    )
                )

    dissolve_left(
            doc,
            Point(3, 4) * FLAGS.scale,
            2 * FLAGS.scale,
            1 * FLAGS.scale,
            )

    dissolve_left(
            doc,
            Point(9, 0) * FLAGS.scale,
            2 * FLAGS.scale,
            1 * FLAGS.scale,
            )


    dissolve_down(
            doc,
            Point(16, 3) * FLAGS.scale,
            1 * FLAGS.scale,
            2 * FLAGS.scale,
            )

    if FLAGS.output == "-":
        output = io.StringIO()
        doc.write(output, pretty=True)
        print(output.getvalue())
    else:
        doc.saveas(FLAGS.output, pretty=True)


if __name__ == "__main__":
    app.run(main)
