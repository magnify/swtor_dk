/*
 * $Id: cutemenu.js,v 1.1.2.3 2009/02/16 01:46:41 alexiswilke Exp $
 *
 * Javascript handling the Cute Menus.
 */

output_element = null;

function cutemenu()
{
	this.menubar = null;
	this.id = -1;
	this.dropdown = null;
	this.timer = -1;
	this.cutemenu_count = 0;	// for debug purposes
}

cutemenu.prototype.show = function(menubar, id, has_dropdown)
{
	if(!output_element) {
		output_element = document.getElementById('OUTPUT');
	}

	this.inside();
	if(this.menubar != menubar) {
		// close another menu before opening the new one
		this.hide();

		// now initialize the new menu
		this.menubar = menubar;
		this.id = id;
		if(has_dropdown) {
			// it is assumed that the dropdown does not change over time
			this.dropdown = document.getElementById('cutemenu-dropdown-' + id);
		}
		else {
			this.dropdown = null;
		}
	}

	if(this.menubar.className.indexOf('cutemenu-active') < 0) {
		if(this.dropdown) {
			this.dropdown.style.display = 'block';
		}
		this.menubar.className += ' cutemenu-active';
	}
//this.cutemenu_count = 0;
//this.cutemenu_count++;
//output_element.innerHTML = menubar.className + ' ' + cutemenu_count + ' ' + menubar.id;
}

cutemenu.prototype.hide = function()
{
	this.inside();
	if(this.menubar && this.menubar.className.indexOf('cutemenu-active') >= 0) {
		if(this.dropdown) {
			this.dropdown.style.display = 'none';
		}
		this.menubar.className = this.menubar.className.replace(/ cutemenu-active/gi, '');
	}
//this.cutemenu_count = 0;
//this.cutemenu_count++;
//output_element.innerHTML = menubar.className + ' ' + cutemenu_count + ' ' + menubar.id;
}

cutemenu.prototype.inside = function()
{
	if(this.timer != -1) {
		window.clearTimeout(this.timer);
		this.timer = -1;
	}
}

cutemenu.prototype.outside = function()
{
	if(this.timer == -1) {
		// this is really ugly, but necessary for IE to work
		// (i.e. the setTimeout() 3rd parameter does not work under IE)
		this_cutemenu_object = this;
		this.timer = window.setTimeout(arguments.callee, 250);
	}
	else if(this_cutemenu_object) {
		this_cutemenu_object.hide();
	}
}


cutemenu_object = new cutemenu;

function cutemenu_mouseover(menubar, id, has_dropdown)
{
	cutemenu_object.show(menubar, id, has_dropdown);
}

function cutemenu_mouseout()
{
	cutemenu_object.outside();
}



function cutemenu_subitem_mouseover(submenu)
{
	submenu.className += ' cutemenu-subitem-active';
}

function cutemenu_subitem_mouseout(submenu)
{
	submenu.className = submenu.className.replace(/ cutemenu-subitem-active/gi, '');
}

// vim: ts=4 sw=4
