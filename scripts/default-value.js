(function ($) {

Drupal.behaviors.defaultValue = {
 attach: function (context, settings) {
    $('input.default-value', context).once('default-value-processed', Drupal.defaultValue);
	
  }
};

Drupal.defaultValue = function () {
//
	var dVal;
	dVal = $(this).val();
	
	$(this).focus(function (event) {
		if ($.trim($(this).val()) == dVal) $(this).val("");
	});
	$(this).blur(function (event) {
		if ($.trim($(this).val()) == "") $(this).val(dVal);
	});
};

})(jQuery);
