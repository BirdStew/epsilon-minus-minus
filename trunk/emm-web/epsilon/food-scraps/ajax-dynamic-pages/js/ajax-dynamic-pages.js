/************************************************************************************************************
Ajax dynamic pages
Copyright (C) 2006  DTHMLGoodies.com, Alf Magne Kalleland

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Dhtmlgoodies.com., hereby disclaims all copyright interest in this script
written by Alf Magne Kalleland.

Alf Magne Kalleland, 2006
Owner of DHTMLgoodies.com
	
************************************************************************************************************/	



/************************************************************************************************************<br>
<br>
	@fileoverview
	DHTMLgoodies_scrollingPages class<br>
	(C) www.dhtmlgoodies.com, October 2006<br>
	<br>
	This is a script from www.dhtmlgoodies.com. You will find this and a lot of other scripts at our website.	<br>
	<br>
	<br>
	Thank you!<br>
	<br>
	www.dhtmlgoodies.com<br>
	Alf Magne Kalleland<br>
<br>
************************************************************************************************************/

// {{{ Constructor
function DHTMLgoodies_scrollingPages()
{
	var targetId;
	var url;
	var sackObj;
	var currentUrl;
	var pageObjects;
	var maximumNumberOfPages;
	var scrollSpeed;
	var currentScrollTop;
	
	this.pageObjects = new Array();
	this.currentUrl = false;
	this.scrollSpeed = 20;
	this.currentScrollTop = 0;
	
	try{
		this.sackObj = new sack();
	}catch(e){
		alert('You need to include the AJAX(SACK) js file');
	}
	
}

// }}}
DHTMLgoodies_scrollingPages.prototype = {

	// {{{ setTargetId()
    /**
     * Specify where to insert dynamic pages.
     *
     * @param String targetId = Id of element where the dynamic pages will be inserted inside.
     * @public
     */		
	setTargetId : function(targetId)
	{
		this.targetId = targetId;		
	}
	// }}}	
	,
	// {{{ setUrl()
    /**
     * Set auto scroll speed
     *
     * @param String url = Specify url of page to load
     * @public
     */		
	setUrl : function(url)
	{
		this.url = url;
	}
	// }}}	
	,
	
	loadPage : function()
	{
		if(!this.url)return;
		if(this.url == this.currentUrl)return;
		this.currentUrl = this.url;
		this.currentScrollTop = Math.max(document.documentElement.scrollTop,document.body.scrollTop);
		var index = this.pageObjects.length;
		
		if(this.maximumNumberOfPages){
			if(document.getElementById('DHTMLgoodies_scrollingPages_page' + (index - this.maximumNumberOfPages))){
				var obj = document.getElementById('DHTMLgoodies_scrollingPages_page' + (index - this.maximumNumberOfPages));
				obj.parentNode.removeChild(obj);
			} 				
		}		
		
		this.pageObjects[index] = document.createElement('DIV');
		this.pageObjects[index].id = 'DHTMLgoodies_scrollingPages_page' + index;
		document.getElementById(this.targetId).appendChild(this.pageObjects[index]);
		window.refToScrollPageObj = this;
		ajax_loadContent('DHTMLgoodies_scrollingPages_page' + index,this.url,'window.refToScrollPageObj.scroll(' + index + ')');
	}
	// }}}	
	,
	// {{{ setMaximumNumberOfPages()
    /**
     * Set auto scroll speed
     *
     * @param Int maximumNumberOfPages = Maximum number of dynamic "pages". When maximum is achieved, the first "page" will be deleted dynamically.
     * @public
     */			
	setMaximumNumberOfPages : function(maximumNumberOfPages)
	{
		this.maximumNumberOfPages = maximumNumberOfPages;
	}
	
	,
	// {{{ setScrollSpeed()
    /**
     * Set auto scroll speed
     *
     * @param Int scrollSpeed = Scroll speed - (NB! Lower = faster)
     * @public
     */		
	setScrollSpeed : function(scrollSpeed)
	{
		this.scrollSpeed = scrollSpeed;
	}
	// }}}	
	,
	
	scroll : function(index)
	{
		window.scrollTo(0,this.currentScrollTop);
		var obj = document.getElementById('DHTMLgoodies_scrollingPages_page' + index);
		var scrollTop = Math.max(document.documentElement.scrollTop,document.body.scrollTop);		
		var scrollTo = this.getTopPos(obj) + obj.offsetHeight;		
		if(scrollTo>scrollTop)this.__performScroll(scrollTo);
		
	}
	
	,
	// {{{ setScrollSpeed()
    /**
     * Set auto scroll speed
     *
     * @param Int scrollSpeed = Scroll speed - (NB! Lower = faster)
     * @private
     */		
	__performScroll : function(scrollTo)
	{
		var scrollTop = Math.max(document.documentElement.scrollTop,document.body.scrollTop);
		var initScrollTop = scrollTop;
		var rest = scrollTo - scrollTop;
		scrollSpeed = Math.round(rest / this.scrollSpeed);
		if(scrollSpeed<1)scrollSpeed = 1;
		scrollTop = scrollTop + scrollSpeed;
		window.scrollTo(0,scrollTop);

		scrollTop = Math.max(document.documentElement.scrollTop,document.body.scrollTop);
		window.refToScrollPageObj = this;
		if(scrollTop < scrollTo && scrollTop!=initScrollTop)setTimeout('window.refToScrollPageObj.__performScroll(' + scrollTo + ')',10);
		
		
	}	
	,
	// {{{ getTopPos()
    /**
     * This method will return the top coordinate(pixel) of an object
     *
     * @param Object inputObj = Reference to HTML element
     * @public
     */	
	getTopPos : function(inputObj)
	{		
	  var returnValue = inputObj.offsetTop;
	  while((inputObj = inputObj.offsetParent) != null){
	  	if(inputObj.tagName!='HTML'){
	  		returnValue += inputObj.offsetTop;
	  		if(document.all)returnValue+=inputObj.clientTop;
	  	}
	  } 
	  return returnValue;
	}
	// }}}	
	,	
	// {{{ getLeftPos()
    /**
     * This method will return the left coordinate(pixel) of an object
     *
     * @param Object inputObj = Reference to HTML element
     * @public
     */	
	getLeftPos : function(inputObj)
	{	  
	  var returnValue = inputObj.offsetLeft;
	  while((inputObj = inputObj.offsetParent) != null){
	  	if(inputObj.tagName!='HTML'){
	  		returnValue += inputObj.offsetLeft;
	  		if(document.all)returnValue+=inputObj.clientLeft;
	  	}
	  }
	  return returnValue;
	}
		
}
