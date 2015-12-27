if(!'config' in localStorage || JSON.parse(localStorage.getItem('config')) == null){

  var DEFAULTS = {
    'BACKGROUND_CHANGE_ENABLED': true,
    'BT_VIBRATE_ENABLED': false,
    'DATE_ENABLED': false    
  }

  localStorage.setItem('config', JSON.stringify(DEFAULTS));
}

var initialized = false;
var config_dict = JSON.parse(localStorage.getItem('config'));

var options = ['BACKGROUND_CHANGE_ENABLED', 'DATE_ENABLED', 'BT_VIBRATE_ENABLED'];

Pebble.addEventListener("ready", function() {
  initialized = true;
});

Pebble.addEventListener("showConfiguration", function() {
  console.log("showing configuration");

  var config_clone = JSON.parse(JSON.stringify(config_dict));
  config_clone.options = options;

  Pebble.openURL('http://dmitrydodzin.github.io/PebbleWave/config/index.html?options='+encodeURIComponent(JSON.stringify(config_clone)));
});

Pebble.addEventListener("webviewclosed", function(e) {
  // webview closed
  //Using primitive JSON validity and non-empty check
  var config_dict_raw = {};
  if (e.response.charAt(0) == "{" && e.response.slice(-1) == "}" && e.response.length > 5) {
    config_dict_raw = JSON.parse(decodeURIComponent(e.response));
    console.log("Recived Config: " + JSON.stringify(config_dict_raw));
  } else {
    console.log("Cancelled");
  }

  // Validation for the fields
  for(key in config_dict_raw){
    if(options.indexOf(key) != -1){
      config_dict[key] = config_dict_raw[key]
    }
  }

  localStorage.setItem('config', JSON.stringify(config_dict));


  Pebble.sendAppMessage(config_dict, function() {
    console.log('Send successful: ' + JSON.stringify(dict));
  }, function() {
    console.log('Send failed!');
  });
});