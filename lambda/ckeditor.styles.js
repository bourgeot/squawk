/*
Copyright (c) 2003-2011, CKSource - Frederico Knabben. All rights reserved.
For licensing, see LICENSE.html or http://ckeditor.com/license
*/

/*
 * This file is used/requested by the 'Styles' button.
 * 'Styles' button is not enabled by default in DrupalFull and DrupalFiltered toolbars.
 */
if(typeof(CKEDITOR) !== 'undefined') {
CKEDITOR.addStylesSet( 'drupal',
[
	/******************
	    Block Styles 
	 ******************/

	// These styles are already available in the "Format" combo, so they are
	// not needed here by default. You may enable them to avoid placing the
	// "Format" combo in the toolbar, maintaining the same features.
	/*
	{ name : 'Paragraph'		, element : 'p' },
	{ name : 'Heading 1'		, element : 'h1' },
	{ name : 'Heading 2'		, element : 'h2' },
	{ name : 'Heading 3'		, element : 'h3' },
	{ name : 'Heading 4'		, element : 'h4' },
	{ name : 'Heading 5'		, element : 'h5' },
	{ name : 'Heading 6'		, element : 'h6' },
	{ name : 'Preformatted Text'	, element : 'pre' },
	{ name : 'Address'		, element : 'address' },
	*/
	/*
	{ name : 'UA Blue Title'	, element : 'h2', styles : { 'color' : '#003366' } },
	{ name : 'UA Red Title'		, element : 'h2', styles : { 'color' : '#cc0033' } },
	{ name : 'Tucson Tan 1 Title'	, element : 'h2', styles : { 'color' : '#e5c19f' } },
	{ name : 'Tucson Tan 2 Title'	, element : 'h2', styles : { 'color' : '#c88a11' } },
	{ name : 'Golden Sun Title' 	, element : 'h2', styles : { 'color' : '#ffd18b' } },
	{ name : 'Town Brown Title' 	, element : 'h2', styles : { 'color' : '#572700' } },
	{ name : 'Sunset Violet Title'	, element : 'h2', styles : { 'color' : '#781d7e' } },
	{ name : 'Rose Violet Title'	, element : 'h2', styles : { 'color' : '#d492b3' } },
	{ name : 'Squash Blossom Title'	, element : 'h2', styles : { 'color' : '#008c7f' } },
	{ name : 'Succulent Green Title', element : 'h2', styles : { 'color' : '#93a445' } },
	*/
	
	/********************
	    Inline Styles
	*********************/
	// These are core styles available as toolbar buttons. You may opt enabling
	// some of them in the Styles combo, removing them from the toolbar.
	/*
	{ name : 'Strong'		, element : 'strong', overrides : 'b' },
	{ name : 'Emphasis'		, element : 'em', overrides : 'i' },
	{ name : 'Underline'		, element : 'u' },
	{ name : 'Strikethrough'	, element : 'strike' },
	{ name : 'Subscript'		, element : 'sub' },
	{ name : 'Superscript'		, element : 'sup' },
	*/
	
	{ name : 'Sub Heading'	, element : 'h2', attributes : { 'class' : 'sub-head' } },
  { name : 'Description'		, element : 'span', attributes : { 'class' : 'description' } },
	{ name : 'Cited Work'		, element : 'cite' },
	{ name : 'Inline Quotation'	, element : 'q' },
/*	
	{ name : 'UA Blue'		, element : 'span', attributes : { 'class' : 'uablue-txt' } },
	{ name : 'UA Red'		, element : 'span', attributes : { 'class' : 'uared-txt' } },
	{ name : 'Tucson Tan 1'		, element : 'span', attributes : { 'class' : 'tucson-tan1-txt' } },
	{ name : 'Tucson Tan 2'		, element : 'span', attributes : { 'class' : 'tucson-tan2-txt' } },
	{ name : 'Golden Sun'		, element : 'span', attributes : { 'class' : 'golden-sun-txt' } },
	{ name : 'Town Brown'		, element : 'span', attributes : { 'class' : 'town-brown-txt' } },
	{ name : 'Sunset Violet'	, element : 'span', attributes : { 'class' : 'sunset-viole-txtt' } },
	{ name : 'Rose Violet'		, element : 'span', attributes : { 'class' : 'rose-violet-txt' } },
	{ name : 'Squash Blossom'	, element : 'span', attributes : { 'class' : 'squash-blossom-txt' } },
	{ name : 'Succulent Green'	, element : 'span', attributes : { 'class' : 'succulent-green-txt' } },
*/
/*
	{ name : 'Marker: Yellow'	, element : 'span', styles : { 'background-color' : '#ffd18b' } },
	{ name : 'Marker: Green'	, element : 'span', styles : { 'background-color' : '#93a445' } },
*/
/*
	{ name : 'Big'			, element : 'big' },
	{ name : 'Small'		, element : 'small' },
	{ name : 'Typewriter'		, element : 'tt' },

	{ name : 'Computer Code'	, element : 'code' },
	{ name : 'Keyboard Phrase'	, element : 'kbd' },
	{ name : 'Sample Text'		, element : 'samp' },
	{ name : 'Variable'		, element : 'var' },

	{ name : 'Deleted Text'		, element : 'del' },
	{ name : 'Inserted Text'	, element : 'ins' },
*/

/*	{ name : 'Language: RTL'	, element : 'span', attributes : { 'dir' : 'rtl' } },
	{ name : 'Language: LTR'	, element : 'span', attributes : { 'dir' : 'ltr' } },
*/
	/* Object Styles */

	{
		name : 'Image on Left',
		element : 'img',
		attributes :
		{
			'style' : 'float: left; padding: 8px; margin-right: 1em; margin-bottom: 1em;',
			'border' : '0 none',
		}
	},

	{
		name : 'Image on Right',
		element : 'img',
		attributes :
		{
			'style' : 'float: right; padding: 8px; margin-left: 1em; margin-bottom: 1em;',
			'border' : '0 none',
		}
	},
	
		{
		name : 'Blockquote Left',
		element : 'p',
		attributes :
		{
			'class' : 'quote-left',
		}
	},

	{
		name : 'Blockquote Right',
		element : 'p',
		attributes :
		{
			'class' : 'quote-right',
		}
	},
	
]);
}