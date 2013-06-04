(function ($) {

Drupal.behaviors.showNodeContent = {
 attach: function (context, settings) {
    $('.summary-button', context).once('show-node-content', Drupal.showNodeContent);
  }
};

Drupal.showNodeContent = function () {
//

	$('.summary-button').click(function (event) {
		var nid = $(this).attr('id').substr(14);
		var $target = $(event.target);
		if ($(this).is('.show-less')) {
			//var nid = $(this).attr('id').substr(14);
			//alert(nid);
			alert('here');
			$(this).removeClass('show-less').addClass('show-more').attr('value','Show More').html('Show More');
			$('[id="teaser' + nid + '"]').slideDown('fast');
			//$('[id="teaser' + nid + '"]').show();
			//$('[id$="' + nid + '"].summary-processed').hide();
			$('[id="full-body' + nid + '"].summary-processed').slideUp();
			$('[id="original-content' + nid + '"]').slideUp();
			event.stopPropagation();
		} 
		else {
			alert('there');
			//var nid = $(this).attr('id').substr(14);
			$(this).removeClass('show-more').addClass('show-less').attr('value','Show Less').html('Show Less');
			$('[id="teaser' + nid + '"]').hide();
			$('[id="full-body' + nid + '"].summary-processed').slideDown('fast');
			$('[id="original-content' + nid + '"]').slideDown();
			event.stopPropagation();
			//$('[id="teaser' + nid + '"]').hide();
			//$('[id$="' + nid + '"].summary-processed').show();

		}
		//$('[id="original-content' + nid + '"]').slideToggle();
	});
};

})(jQuery);
