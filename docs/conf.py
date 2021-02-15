
# -- Project information -----------------------------------------------------

project = 'Ohmcha'
copyright = '2020, Haris Gušić, Anes Hadžić'
author = 'Haris Gušić, Anes Hadžić'

# -- General configuration ---------------------------------------------------

extensions = [
        'breathe', 'sphinx.ext.todo', 'sphinx.ext.mathjax',
        'sphinx.ext.autosectionlabel'
]

# -- Breathe configuration ---------------------------------------------------

breathe_projects = { "Ohmcha": "doxygen/xml/" }
breathe_default_project = "Ohmcha"
# Stop breathe from ignoring main.cpp
breathe_implementation_filename_extensions = []
breathe_default_members = ()

primary_domain = 'cpp'
highlight_language = 'cpp'

# Build doxygen from here -- ReadTheDocs requires this
import subprocess
subprocess.call('mkdir -p _build/doxygen', shell=True)
subprocess.call('doxygen', shell=True)
#subprocess.call('cd _build/ && mkdir -p html/doxygen && cp -r doxygen/html/* html/doxygen',
        #shell=True)

# -- Options for HTML output -------------------------------------------------

html_theme = 'sphinx_rtd_theme'

html_static_path = ['_static']
# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

todo_include_todos = True
