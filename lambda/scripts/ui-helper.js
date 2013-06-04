(function ($) {
$(document).ready(function() {

$('.form-checkboxes input[type=checkbox]:checked').parent().addClass('tag-checked');
$('.form-checkboxes input[type=checkbox]:not(:checked)').parent().removeClass('tag-checked');

$('.form-checkboxes input[type=checkbox]').change(function(){
	$(this).parent().toggleClass('tag-checked', this.checked);
});


});
}(jQuery));