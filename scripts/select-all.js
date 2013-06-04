(function ($) {

Drupal.behaviors.selectAll = {
 attach: function (context, settings) {
    // Select the closest fieldset.
    $('input:checkbox.select-all', context).closest('fieldset').once('select-all', Drupal.selectAll);
  }
};

Drupal.selectAll = function () {
//



  // Keep track of the fieldset, which checkbox is checked and alias the settings.
  var thefieldset = this;
  var checkboxes;
  var strings = { 'selectAll': Drupal.t('Select all checkboxes in this field set'), 'selectNone': Drupal.t('Deselect all checkboxes in this field set') };
  var updateSelectAll = function (state) {
    // Update table's select-all checkbox (and sticky header's if available).
    $(thefieldset).find('input.select-all:checkbox').each(function() {
      $(this).attr('title', state ? strings.selectNone : strings.selectAll);
      this.checked = state;
	});
  };

  $('input:checkbox.select-all', thefieldset).click(function (event) {
      if ($(event.target).is('input:checkbox')) {
	  // Loop through all checkboxes and set their state to the select all checkbox' state.
		checkboxes.each(function () {
			this.checked = event.target.checked;
			// Either add or remove the selected class based on the state of the check all checkbox.
			$(this).toggleClass('selected', this.checked);
			$(this).closest('div').toggleClass('tag-checked', this.checked);
		});
      // Update the title and the state of the check all box.
		updateSelectAll(event.target.checked);
	  }
  });

  // For each of the checkboxes within the table that are not disabled.
  checkboxes = $('div.form-type-checkbox input:checkbox:enabled', thefieldset).not('.select-all').click(function (e) {
    // Either add or remove the selected class based on the state of the check all checkbox.
    $(this).toggleClass('selected', this.checked);
	$(this).parent().toggleClass('tag-checked', this.checked);
    // If all checkboxes are checked, make sure the select-all one is checked too, otherwise keep unchecked.
    updateSelectAll((checkboxes.length == $(checkboxes).filter(':checked').length));

    // Keep track of the last checked checkbox.
    lastChecked = e.target;
  });
};

})(jQuery);
