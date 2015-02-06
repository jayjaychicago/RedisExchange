<!DOCTYPE html>
<?php
session_start();
$email = $_SESSION["email"];

function connect_redis() {
        //echo "Connecting to redis...";
        require_once 'predis/autoload.php';
         
        $single_server = array('host' => '127.0.0.1', 'port' => 6379, 'database' => 15);
         
        $multiple_servers = array( array('host' => '127.0.0.1', 'port' => 6379, 'database' => 15, 'alias' => 'first', ), array('host' => '127.0.0.1', 'port' => 6380, 'database' => 15, 'alias' => 'second', ), );
             
        // simple set and get scenario
        $time_start = microtime(true);
        $_SESSION["client"] = new Predis\Client($single_server);
}   
    
connect_redis();

if ($email != "") { 
        $exchangeName = $_SESSION["client"]->GET("FIRSTEXCHANGE:" . $email);
        $productName = $_SESSION["client"]->GET("FIRSTPRODUCT:" . $email);
        header('Location: /lumino/sbc.php?exchange=' . $exchangeName . '&product=' . $productName);
}
?>
<html>
  <head>
    <title>Instabid: Register</title>

      <link rel="stylesheet" href="/manage/assets/application.manage-5f7b52cfc85769c67834855af5cd18a5.css">
<!--[if lte IE 9]>
<![endif]-->
<link rel="shortcut icon" href="/favicon.ico">



    <link rel="stylesheet" href="/css/fonts.css">
<script src="/manage/assets/jquery-47d53801b9f61ca8a21dcc2571aeb3ec.js"></script>
    <script src="/manage/assets/modernizr-16114bbaf42d14126fb991d11fc0b961.js"></script>
    <script src="/manage/assets/shared-3abadcaa3af6e785a085062e45894014.js"></script>
    <script src="/manage/assets/config-aead70c07cdca45cec67e4141bf18edc.js"></script>
    <script src="/manage/assets/password-indicator-43e2996773e71e0b05739d6865dd7c4c.js"></script>
    <script type="application/json" id="register_json">false</script>
    <!--<script src="/manage/assets/register-43a7266fe4a6b0fbdd363024e60cba7c.js"></script>-->
    <script src="/manage/assets/instabid.js"></script>
    <!-- <script src="/manage/assets/register-julien.js"></script> -->
    <script src="/manage/assets/zxcvbn-8143efb328ef1456bebfa7ebd1a127be.js" async></script>
    <!--[if lt IE 9]>
    <![endif]-->
    <script type="application/json" id="mixpanel_config">{&quot;identifier&quot;:&quot;eb71b6171a4f7ed97de9b7a0395b7ca5&quot;}</script>
<script type="application/json" id="analytics_config">{}</script>
<script src="https://www.googleadservices.com/pagead/conversion.js"></script>
<script src="/manage/assets/external_analytics-a9e3d39cd6110aa8c184d9c1f2e5777e.js"></script>
<script src="/manage/assets/qframe-cb903dc9166e266dc7d9f11d44f5e81a.js"></script>
<SCRIPT>


jQuery(document).ready(function ($) {
$("#register_button").click(function() {
		if ($("#password").val() != $("#password2").val()) {
			$("#niceTitle").append("<p style='color:red;'>The 2 passwords you entered did not match</p>");
		}
		else {
			$.ajax({type:"POST",
			url:"register_process.php",
			cache:false,
			data:$("#register_form").serializeArray(),
			success: function (data,textStatus,jqXHR) {
				if (data.substring(0,7)=="SUCCESS") {
						window.location.replace("/lumino/sbc.php" + data.substring(8) + "&firstTime=1");
					      }
				else
					{
				$("#niceTitle").append("<p style='color:red;'>" + data.substring(6) + "</p>");
                                }
			},
			error: function (request, status, error) {
				alert(request.responseText);
								 }
			});
		}
			});
});


</SCRIPT>
  </head>

  <body id="login" class="register-page">

    <div id="login-background"><div class="inner"></div></div>



     <div id="wrap">

     <!-- Header -->
     <div id="header">
         <h1><a href="/">Instabid</a></h1>
     </div>

      <div id="main-body" class="box css"><div class="inner">

        <div class="title extra">
                 <h2 id="niceTitle">Welcome, please register below</h2>
            <!--<p>
                &hellip;or save your account later, and <a id="skip-this-step" href="/anonymous" class="arrow">skip this step</a>
            </p> -->
        </div>

          <form id="register_form" method="POST" action="register_process.php">
              <p class="text">
                  <span>
                      <label for="email">Email</label>
                      <input type="email" value="" name="email" id="email" placeholder="Email" />
                  </span>
              </p>
              <p class="text">
                  <span>
                      <label for="password">Password</label>
                      <input type="password" value="" name="password" id="password" placeholder="Password" class="password-strength-input" />
                  </span>
              </p>
              <p class="text">
                  <span>
                      <label for="confirm_password">Confirm Password</label>
                      <input type="password" value="" name="password2" id="password2" placeholder="Confirm password" />
                  </span>
              </p>


              <input type="hidden" name="redirect" value="" />
              <input type="hidden" name="invite_code" value="" />
              <input type="hidden" name="invite" value="" />
              <input type="hidden" name="country" value="" />
              <input type="hidden" name="csrf_token" value="-ymtmTooAdm5ZbYtLThjwc1eROolBW5DYi9E0UiNTsV41rcNYqlmjU9EBFdpWaQM3vClAKLz0CIQsK34NX5y6A=="/>

              <div>
                  <p class="submit">
                      <button type="submit" onclick="return false;" class="button blue medium" id="register_button"><span>Register</span></button>
                  </p>
                  <p class="remember" style="display:none">
                      <label><input type="checkbox" name="remember" checked="checked" /> Remember me</label>
                  </p>
              </div>
                <script src="/manage/assets/iovation_config-1195793ef31241e2149d6a7b2d3658ba.js"></script>
  <script src="https://mpsnare.iesnare.com/snare.js"></script>
  <input type="hidden" name="io_blackbox" id="io_blackbox"/>

          </form>

      </div></div> <!-- END CONTENT -->

      <div class="below-box">
        <p>
          Already have an account?
          <a href="/login_instabid.php">Sign in</a>.
      </p>
      </div>

      <div id="footer_spacer"></div>
        <div id="bottom">
          <ul>
              <li class="first"><a href="/reset">Forgot your password?</a></li>
              <li><a href="/privacy">Privacy Policy</a></li>
              <li><a href="mailto:support@instabid.org">Contact</a></li>
              <li class="last"><a href="/">&copy; Stripe</a></li>
          </ul>
      </div>
    </div>
    <script src="/manage/assets/footer_analytics-6e8fc3677422b1ab0ef7aca057103605.js"></script>

<noscript>
  <div style="display:inline;">
  <img height="1" width="1" style="border-style:none;" alt="" src="https://googleads.g.doubleclick.net/pagead/viewthroughconversion/1008370075/?value=0&amp;guid=ON&amp;script=0"/>
  </div>
</noscript>

  </body>
</html>
