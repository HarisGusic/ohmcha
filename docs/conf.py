# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = 'Ohmcha'
copyright = '2020, Haris Gušić, Anes Hadžić'
author = 'Haris Gušić, Anes Hadžić'

# -- General configuration ---------------------------------------------------

extensions = [
        'breathe', 'sphinx.ext.todo', 'sphinx.ext.mathjax',
        'sphinx.ext.autosectionlabel'
]

breathe_projects = { "Ohmcha": "doxygen/xml/" }
breathe_default_project = "Ohmcha"
# Stop breathe from ignoring main.cpp
breathe_implementation_filename_extensions = []
breathe_default_members = ()

primary_domain = 'cpp'
highlight_language = 'cpp'

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

todo_include_todos = True


# -- Options for HTML output -------------------------------------------------

html_theme = 'sphinx_rtd_theme'

html_static_path = ['_static']
