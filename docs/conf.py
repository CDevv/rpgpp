import sys
import os

# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'RPG++'
copyright = '2026, RPG++ Contributors'
author = 'RPG++ Contributors'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

sys.path.append('.venv/lib/python3.14/site-packages/breathe/')

extensions = ['sphinx.ext.autodoc', 'sphinx.ext.autosummary', 'breathe', 'sphinx_lua_ls']

templates_path = ['_templates']
exclude_patterns = []

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'furo'
html_static_path = ['_static']

breathe_projects = {"RPG++": "../build/doxygen/xml"}
breathe_default_project = "RPG++"

lua_ls_project_root = "../luasrc"
lua_ls_backend = "luals"
lua_ls_apidoc_roots = {
   "WorldService": "userapi/world",
   "GameState": "userapi/state",
   "Sounds": "userapi/sounds",
   "Interface": "userapi/interface"
}



