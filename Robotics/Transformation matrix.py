#!/usr/bin/env python3
"""Homogeneous transformation matrix for a ZYX (yaw-pitch-roll) rotation.

The rotation part is built as ``R = Rz(alpha) @ Ry(beta) @ Rx(gamma)`` and is
extended to a 4x4 homogeneous transform with the translation (Px, Py, Pz):

    T = | R  P |
        | 0  1 |

Usage:
    python "Transformation matrix.py"            # interactive
    python "Transformation matrix.py" 30 45 60   # gamma beta alpha (degrees)
"""

import sys
from math import cos, sin, radians


def rotation_zyx(alpha, beta, gamma):
    """Return ``Rz(alpha) @ Ry(beta) @ Rx(gamma)`` as three rows (radians)."""
    ca, sa = cos(alpha), sin(alpha)
    cb, sb = cos(beta), sin(beta)
    cg, sg = cos(gamma), sin(gamma)

    return [
        [ca * cb, ca * sb * sg - sa * cg, ca * sb * cg + sa * sg],
        [sa * cb, sa * sb * sg + ca * cg, sa * sb * cg - ca * sg],
        [-sb, cb * sg, cb * cg],
    ]


def homogeneous(rotation, translation):
    """Extend a 3x3 rotation and a translation into a 4x4 matrix."""
    return [row[:] + [value] for row, value in zip(rotation, translation)] + [
        [0.0, 0.0, 0.0, 1.0]
    ]


def determinant3(matrix):
    return (
        matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1])
        - matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0])
        + matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0])
    )


def orthonormality_error(rotation):
    """Largest absolute deviation of ``R @ R.T`` from the identity matrix."""
    error = 0.0
    for i in range(3):
        for j in range(3):
            dot = sum(rotation[i][k] * rotation[j][k] for k in range(3))
            expected = 1.0 if i == j else 0.0
            error = max(error, abs(dot - expected))
    return error


def format_matrix(matrix, indent="  "):
    lines = []
    for row in matrix:
        lines.append(indent + "[" + "  ".join(f"{value:9.4f}" for value in row) + "]")
    return "\n".join(lines)


def read_triple(prompt):
    """Read exactly three numbers from the user, retrying on bad input."""
    while True:
        try:
            raw = input(prompt)
        except EOFError:
            raise SystemExit("\nNo input received, exiting.")

        parts = raw.replace(",", " ").split()
        if len(parts) != 3:
            print("  please enter exactly three numbers, for example: 30 45 60")
            continue

        try:
            return [float(value) for value in parts]
        except ValueError:
            print("  those values are not valid numbers, please try again")


def parse_arguments(argv):
    if not argv:
        return None
    if len(argv) != 3:
        raise SystemExit('usage: python "Transformation matrix.py" [gamma beta alpha]')
    try:
        return [float(value) for value in argv]
    except ValueError:
        raise SystemExit("all three angles must be numbers (degrees)")


def main():
    arguments = parse_arguments(sys.argv[1:])
    if arguments is None:
        gamma, beta, alpha = read_triple(
            "Enter gamma, beta and alpha (degrees, space separated): "
        )
    else:
        gamma, beta, alpha = arguments

    rotation = rotation_zyx(radians(alpha), radians(beta), radians(gamma))

    print("\nRotation matrix R = Rz(alpha) * Ry(beta) * Rx(gamma):")
    print(format_matrix(rotation))

    if arguments is None:
        px, py, pz = read_triple("Enter Px, Py and Pz (space separated): ")
    else:
        px = py = pz = 0.0

    transform = homogeneous(rotation, [px, py, pz])
    print("\nTransformation matrix B -> A:")
    print(format_matrix(transform))

    det = determinant3(rotation)
    error = orthonormality_error(rotation)

    print("\nVerification:")
    print(f"  det(R)             = {det:.6f}   (expected 1)")
    print(f"  max |R * R^T - I|  = {error:.2e}   (expected ~0)")

    if abs(det - 1.0) < 1e-9 and error < 1e-9:
        print("  PASS: R is a valid rotation matrix.")
    else:
        print("  FAIL: R is not a rotation matrix.")


if __name__ == "__main__":
    main()
