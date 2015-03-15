$(document).ready(function() {
	$("button[type='submit']").click(function() {
		var email = $("#email").val();
		var password = $("#password").val();
		console.log('email contains: ' + email);
		var result = createExchange(email, password, function(data) {
			console.log(data);
		});
});

});
