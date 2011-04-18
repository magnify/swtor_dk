$Id: README.txt,v 1.1.2.2 2009/09/11 07:46:06 alexiswilke Exp $

Name: cutemenu
Author: Alexis Wilke <alexis@m2osw.com>
Dependencies: menu module (core module)


Description
===========

The CuteMenu is a module that generates a menu with many
<div> tags instead of the usual <ul> + <li> list.

This lets theme developers create incredibly advanced
looks for their menu.

CuteMenu also comes with a drop down feature. The top
menu items appear in what looks like a menubar. When
moving the mouse over these items, a dropdown shows
up (Javascript is required.)


Installation
============

Extract the CuteMenu module under your sites/all/modules
directory.

To start using the CuteMenu, go to Admin => Site Building => Blocks
and select the CuteMenu blocks for display. You may want to create
a new menu first. See Admin => Site Building => Menus.

Edit your theme to taste.


Requirements
============

* Menu module (comes with Drupal Core)

* Javascript enabled in your browser

* Knowledge on how to write CSS code to tailor your CuteMenus


Problems
========

The default cutemenu.css cannot always properly overwrite all the
parameters defined in a theme. This means the default CuteMenu
will not work properly in all themes. The only way around this
problem is to edit your CSS and make the CuteMenu work properly
using your own CSS code.

IE 6.0
======

By default, IE 6.0 is not supported. However, there is a fix-ie.css
file available to you in this module package.

Because there is no way to automatically include that CSS file for
a specific version of a browser, you will have to edit the theme(s)
in which you want to use CuteMenu with IE 6.0 and add the following
between the <head> and </head> tag delimiters:

  <!--[if lte IE 6]>
  <link type="text/css" rel="stylesheet" media="all" href="/sites/all/modules/cutemenu/fix-ie.css"/>
  <![endif]-->

This will include the fix for Internet Explorer version 6 or less.

Note that the CSS simply forces one of the <div> tag to a specific
width. If necessary, edit the fix-ie.css file and change the fixed
width (which applies to all the top menu entries.)

