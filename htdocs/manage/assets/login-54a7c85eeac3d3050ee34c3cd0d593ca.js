$(document).ready(function(){var e=$("div#wrap"),t=e.height(),n=parseInt(e.css("padding-top").replace("px","")),r=n+t+40,i=function(){var t=$(window).height();if(r>t){var i=r-t;i>n&&(i=n),e.css("padding-top",n-i)}},s=function(){$.browser.safari&&$("input[value='']").first().focus()};i(),s(),window.onresize=i,$("form").submit(function(){var e=function(e){var t;e.hasOwnProperty("responseText")?t=$.parseJSON(e.responseText).error:t=e.error,t.error_type==="need_otp"||t.error_type==="bad_otp"?(t.error_type==="need_otp"&&!$(".second-step").is(":visible")?$(".second-step h2").removeClass("error").text("Enter your security code"):$(".second-step h2").addClass("error").text(t.message),$(".first-step").addClass("hidden"),$(".second-step").removeClass("hidden"),$("#otp").focus().select()):($(".first-step h2").addClass("error").text(t.message),$(".first-step").removeClass("hidden"),$(".second-step").addClass("hidden")),t.message.toLowerCase()=="wrong email or password."&&$(".first-step h2").append($("<a href='/reset' style='display:block'>Forgot your password?</a>")),$('button[type="submit"]').removeClass("disabled")};return $('button[type="submit"]').addClass("disabled"),$.ajax({type:"POST",url:"/ajax/sessions",data:$("form").serialize(),dataType:"json",success:function(t){t.hasOwnProperty("error")?e(t):window.location=t.redirect},error:function(t){e(t)}}),!1})}),Analytics.track("site.login.viewed");