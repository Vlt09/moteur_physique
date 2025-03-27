import io
import shutil
import subprocess
import zipfile
from pathlib import Path

import httpx
import pytest


def download_and_unzip(url: str, extract_to: Path):
    # Download the file
    response = httpx.get(url)
    response.raise_for_status()  # Check if the download was successful

    # Unzip the file
    with zipfile.ZipFile(io.BytesIO(response.content)) as z:
        z.extractall(extract_to)


def test_clone_gltf_sample_models(gltf_models_dir):
    shutil.rmtree(gltf_models_dir, ignore_errors=True)
    cp = subprocess.run(
        [
            "git",
            "clone",
            "https://github.com/KhronosGroup/glTF-Sample-Models",
            f"{gltf_models_dir}",
        ],
        shell=True,
    )
    assert cp.returncode == 0


@pytest.fixture
def gltf_out_dir(gltf_models_dir):
    out_dir = gltf_models_dir / "2.0"
    out_dir.mkdir(parents=True, exist_ok=True)
    return out_dir


def test_get_sponza_sample(gltf_out_dir):
    download_and_unzip(
        "https://gltf-viewer-tutorial.gitlab.io/assets/Sponza.zip",
        gltf_out_dir,
    )


def test_get_damaged_helmet_sample(gltf_out_dir):
    download_and_unzip(
        "https://gltf-viewer-tutorial.gitlab.io/assets/DamagedHelmet.zip",
        gltf_out_dir,
    )
