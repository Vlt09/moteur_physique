from pathlib import Path

import pytest


@pytest.fixture
def root_dir():
    return Path(__file__).parent.parent


@pytest.fixture
def build_dir(root_dir):
    return root_dir / "build"


@pytest.fixture
def dist_dir(root_dir):
    return root_dir / "dist"


@pytest.fixture
def gltf_models_dir(root_dir):
    return root_dir / "gltf-sample-models"
