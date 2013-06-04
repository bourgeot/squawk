(function ($) {

Drupal.behaviors.showNodeContent = {
 attach: function (context, settings) {
	$('#squawk-updates-container').once('show-node-content', Drupal.showNodeContent);
	//by attaching this to the main div, and then delegating the event, we can ensure that it only runs once.
	//otherwise, any click will cause the event to repeat a number of times equal to the elements on the page (in this  case,
	//the numbe rof buttons.
  }
};

Drupal.showNodeContent = function () {
	$('#squawk-updates-container').delegate("button", "click", function() {
		var nid = $(this).attr('id').substr(14);
		if ($(this).is('.show-less')) {
			$(this).removeClass('show-less').addClass('show-more').attr('value','Show More').html('Show More');
			$('[id="teaser' + nid + '"]').slideDown('fast');
			//$('[id="teaser' + nid + '"]').show();
			//$('[id$="' + nid + '"].summary-processed').hide();
			$('[id="full-body' + nid + '"].summary-processed').slideUp();
			$('[id="original-content' + nid + '"]').slideUp();

		} 
		else {
			$(this).removeClass('show-more').addClass('show-less').attr('value','Show Less').html('Show Less');
			$('[id="teaser' + nid + '"]').hide();
			$('[id="full-body' + nid + '"].summary-processed').slideDown();
			$('[id="original-content' + nid + '"]').slideDown();
			//event.stopPropagation();
			//$('[id="teaser' + nid + '"]').hide();
			//$('[id$="' + nid + '"].summary-processed').show();

		}
	});
};

})(jQuery);
