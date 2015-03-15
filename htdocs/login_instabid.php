<?php
ini_set('display_errors',1);
error_reporting(E_ALL);
session_start();

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

$email = $_SESSION["email"];
if ($email != "") {
	$exchangeName = $_SESSION["client"]->GET("FIRSTEXCHANGE:" . $email);
	$productName = $_SESSION["client"]->GET("FIRSTPRODUCT:" . $email);
	header('Location: /lumino/sbc.php?exchange=' . $exchangeName . '&product=' . $productName);
}

?>
<!DOCTYPE html>
<html>
  <head>
    <title>Instabid: Login</title>

      <link rel="stylesheet" href="/manage/assets/application.manage-5f7b52cfc85769c67834855af5cd18a5.css">
<!--[if lte IE 9]>
<![endif]-->
<link rel="shortcut icon" href="/favicon.ico">


    <link rel="stylesheet" href="/css/fonts.css">
    <script src="/manage/assets/jquery-47d53801b9f61ca8a21dcc2571aeb3ec.js"></script>
    <script src="/manage/assets/modernizr-16114bbaf42d14126fb991d11fc0b961.js"></script>
    <script src="/manage/assets/shared-3abadcaa3af6e785a085062e45894014.js"></script>
    <script src="/manage/assets/config-aead70c07cdca45cec67e4141bf18edc.js"></script>
    <!--[if lt IE 9]>
    <![endif]-->
    <script type="application/json" id="mixpanel_config">{&quot;identifier&quot;:&quot;eb71b6171a4f7ed97de9b7a0395b7ca5&quot;}</script>
<script type="application/json" id="analytics_config">{}</script>
<script src="https://www.googleadservices.com/pagead/conversion.js"></script>
<script src="/manage/assets/external_analytics-a9e3d39cd6110aa8c184d9c1f2e5777e.js"></script>
<script src="/manage/assets/qframe-cb903dc9166e266dc7d9f11d44f5e81a.js"></script>

    <script src="/manage/assets/login-54a7c85eeac3d3050ee34c3cd0d593ca.js"></script>
<SCRIPT>

jQuery(document).ready(function ($) {
			$("#niceTitle").hide();
$("#login_button").click(function() {
                        $.ajax({type:"POST",
                        url:"login_process.php",
                        cache:false,
                        data:$("#login_form").serializeArray(),
                        success: function (data,textStatus,jqXHR) {
				var jsonMsg = JSON.parse(data);
                                if (jsonMsg.result == "SUCCESS") {
                                                window.location.replace("/lumino/sbc.php?exchange=" + jsonMsg.exchange + "&product=" + jsonMsg.product);
                                              }
                                else
                                        {
				$("#niceTitle").show();
                                $("#niceTitle").html("<p style='color:red;'>" + jsonMsg.comment + "</p>");
                                }
                        },
                        error: function (request, status, error) {
                                alert(request.responseText);
                                                                 }
                        });
                        });
});


</SCRIPT>
  </head>
  <body id="login">

    <div id="login-background"><div class="inner"></div></div>

    <div id="wrap">

      <!-- Header -->
      <div id="header">
          <h1><a href="/">Instabid</a></h1>
      </div>

      <!-- LOGIN BOX -->
      <div id="main-body" class="box css"><div class="inner">

        <div class="title first-step">
          <h2>Welcome to Instabid, please login:</h2>
	  <h2 id="niceTitle"></h2>
        </div>

        <div class="title second-step extra hidden">
          <h2></h2>
          <p>Open up your <a href="http://support.google.com/accounts/bin/answer.py?hl=en&answer=1066447">Google Authenticator</a> app to view your security code.</p>
        </div>

          <form id="login_form" class="fancy-label" action="/jsbroken" method="post">
            <div class="step first-step">
                  <p class="text">
                      <span>
                          <label for="email">Email</label>
                          <input type="email" value="" name="email" id="email" placeholder="Email" />
                      </span>
                  </p>
                  <p class="text">
                      <span>
                          <label for="password">Password</label>
                          <input type="password" value="" name="password" id="password" placeholder="Password" />
                      </span>
                  </p>
            </div>

            <div class="step second-step hidden">
                  <p class="text fancy-label" id="otp_label">
                      <span>
                          <label for="otp">security code</label>
                          <input type="text" value="" placeholder="6-digit security code" name="otp" id="otp" autocomplete="off" />
                      </span>
                  </p>
                  <p class="text lost-mobile">
                    <span>
                      Lost your mobile device? <a href="/disable-two-factor">Disable verification</a>.
                    </span>
                  </p>
            </div>

            <input type="hidden" name="redirect" value="" />
            <input type="hidden" name="invite_code" value="" />
            <input type="hidden" name="account_invite" value="" />
            <input type="hidden" name="invite" value="" />

            <p class='forgot first-step'>
              <a href="/reset">Forgot your password?</a>
            </p>

            <p class="remember">
                <label><input type="checkbox" name="remember" value="true" checked="checked" /> Remember me</label>
            </p>

            <p class="submit">
              <button id="login_button" class="button blue" type="submit" onclick="return false;"><span>Login to Instabid</span></button>
            </p>


            <input type="hidden" name="csrf_token" value="rpLENQ69IGP19yGv-bv2zobXZHsH5oMvN4gp-wBhqmCm-w7S147U2lP_Dw6PFfRVFVfsmgT5U7zI8iDxgpMItQ=="/>
              <script src="/manage/assets/iovation_config-1195793ef31241e2149d6a7b2d3658ba.js"></script>
  <script src="https://mpsnare.iesnare.com/snare.js"></script>
  <input type="hidden" name="io_blackbox" id="io_blackbox"/>

          </form>

      </div></div>
      <!-- FOOTER -->
      <div id="footer" class="box css">
            <a href="/register">Don't have an account? <span>Sign up</span></a>
      </div><!-- END FOOTER -->
      <div id="footer_spacer"></div>

        <div id="bottom">
          <ul>
              <li class="first"><a href="/privacy">Privacy Policy</a></li>
              <li><a href="mailto:support@instabid.org">Contact</a></li>
              <li class="last"><a href="/">&copy; -</a></li>
          </ul>
      </div>
    </div>
    <script src="/manage/assets/footer_analytics-6e8fc3677422b1ab0ef7aca057103605.js"></script>

<noscript>
  <div style="display:inline;">
  <img height="1" width="1" style="border-style:none;" alt="" src="https://googleads.g.doubleclick.net/pagead/viewthroughconversion/1008370075/?value=0&amp;guid=ON&amp;script=0"/>
  </div>
</noscript>


    <noscript>
                <div class="no-js-view">
                        <div class="position">
                                <div class="icon"></div>
                                <div class="well">
                                        <h1>No love for Javascript?</h1>
                                        <p>Instabid requires your browser to have Javascript enabled. <a href="http://enable-javascript.com" target="_blank" class="arrow">Learn more</a></p>
                                </div>
                                <div class="footer">
                                        <p>&copy; -</p>
                                </div>
                        </div>
                </div>
        </noscript>

  </body>

</html>
