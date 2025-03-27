import shutil
import subprocess

import pytest


def test_cmake_prepare(root_dir, build_dir, dist_dir):
    cp = subprocess.run(
        [
            "cmake",
            "-S",
            f"{root_dir}",
            "-B",
            f"{build_dir}",
            f"-DCMAKE_INSTALL_PREFIX={dist_dir}",
        ],
        shell=True,
    )
    assert cp.returncode == 0


def test_cmake_build(build_dir):
    cp = subprocess.run(
        [
            "cmake",
            "--build",
            f"{build_dir}",
            "-j",
            "--config",
            "debug",
        ],
        shell=True,
    )
    assert cp.returncode == 0


def test_cmake_clean(build_dir, dist_dir):
    shutil.rmtree(build_dir, ignore_errors=True)
    shutil.rmtree(dist_dir, ignore_errors=True)


def test_cmake_install(build_dir):
    cp = subprocess.run(
        [
            "cmake",
            "--build",
            f"{build_dir}",
            "-j",
            "--config",
            "debug",
            "--target",
            "install",
        ],
        shell=True,
    )
    assert cp.returncode == 0
