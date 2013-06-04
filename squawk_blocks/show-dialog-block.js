(function ($) {

Drupal.behaviors.showDialogBlock = {
 attach: function (context, settings) {
    // Select the closest fieldset.
    $('#open-sender-dialog-link', context).once('open-dialog-link', Drupal.showDialogBlock);
  }
};

Drupal.showDialogBlock = function () {
//
	$(function() {
		$('#dialog-senders').dialog({
			autoOpen: false,
			modal: true,
			title: 'Sender List'
		});
	});
	
	$('#open-sender-dialog-link').click(function (event) {
		$('#dialog-senders').dialog('open');
	});
};

Drupal.behaviors.showHideTarget = {
 attach: function (context, settings) {
    // Select the closest fieldset.
    $('.sender-filter-checkbox', context).once('show-hide-target', Drupal.showHideTarget);
  }
};
Drupal.showHideTarget = function () {
	$('.sender-filter-checkbox').click(function (event) {
		var $me = $(event.target);
		if($me = $(event.target)) {
			var sought = $(this).attr('id').substring(16);
			if ($me.is(':checked')) {
				$('#squawk-updates-container div.author-' + sought).removeClass('squawk-hidden');
			}
			else {
				$('#squawk-updates-container div.author-' + sought).addClass('squawk-hidden');
			}
		}
	});
};
})(jQuery);
