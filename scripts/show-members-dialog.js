(function ($) {

Drupal.behaviors.showMembersDialog = {
 attach: function (context, settings) {
    $('#show-members', context).once('show-members-dialog', Drupal.showMembersDialog);
	$("ul.tabs.primary li a:contains('Members')", context).once('show-members-dialog', Drupal.showMembersDialog);
	
  }
};

Drupal.showMembersDialog = function () {
//
	$(function() {
		$('#members-dialog').dialog({
			autoOpen: false,
			modal: true
		});
	});
	
	$('.show-members-dialog-processed').click(function (event) {
		event.preventDefault();
		$('#members-dialog').dialog('open');
	});
};

})(jQuery);
