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
			modal: true
		});
	});
	
	$('#open-sender-dialog-link').click(function (event) {
		$('#dialog-senders').dialog('open');
	});
};

})(jQuery);
