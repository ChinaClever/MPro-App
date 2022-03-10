$('.section-title').click(function() {
	var self = $(this),
		section = self.next('div.section-cont');

	if (!section.is(':animated')) {
		self.find('i').toggleClass('icon-subtraction icon-addition');
		section.slideToggle(260);
	}
});
