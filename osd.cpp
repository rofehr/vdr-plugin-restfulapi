<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xml:lang="en" lang="en" xmlns="http://www.w3.org/1999/xhtml">
  <head>
    <script src="jquery-1.6.2.js"></script>
    <script>

function VdrOsd(){

  //change restfulapiHost according to your needs
  this.restfulapiHost = "http://127.0.0.1:8002/";
  this.waitBeforeRefresh = 300; //milliseconds
  this.oldOsdData = null;

  this.refreshOsdData = function()
  {
    OSDobj = this;
    $.getJSON( this.restfulapiHost + "osd.json", function(data){OSDobj.updateScreen(data)})
    .error(function(data) { OSDobj.setScreenToBlank(); });
  }

  this.setScreenToBlank = function (data)
  {
    this.oldOsdData = null;
    $("div#header").toggleClass("invisible", true);
    $("div#color_buttons" ).toggleClass("invisible", true);
    $("div#content" ).toggleClass("invisible", true);
    //trigger refresh x milliseconds after rendering
    var x = setTimeout("OSDobj.refreshOsdData()", this.waitBeforeRefresh);
  }

  this.updateScreen = function (data)
  {
    var TextOsd = data.TextOsd;
    if (TextOsd.type !== "TextOsd") $("body").html(TextOsd.type);
    if (JSON.stringify(this.oldOsdData) !== JSON.stringify(TextOsd)){
      if (typeof TextOsd.title == "string")
      {
          $("div#header").html(TextOsd.title);
          $("div#header").toggleClass("invisible", false);
      }
      else
      {
          $("div#header").html("");
          $("div#header").toggleClass("invisible", true);
      }

      var colorButtons = [ "red", "green", "yellow", "blue" ];
      var showColorButtons = false;
      var z= 0;
      for (z = 0; z < colorButtons.length; z++)
      {
          var currentButton = "div#" + colorButtons[z];
          if (typeof (TextOsd[ colorButtons[z] ]) == "string" && TextOsd[ colorButtons[z] ] != "")
          {
              $( currentButton ).html(TextOsd[ colorButtons[z] ]);
              $( currentButton ).toggleClass("active", true);
              $( currentButton ).toggleClass("inactive", false);
              showColorButtons = true;
          }
          else
          {
              $( currentButton ).html("&nbsp;");
              $( currentButton ).toggleClass("active", false);
              $( currentButton ).toggleClass("inactive", true);
          }
      }
      $("div#color_buttons" ).toggleClass("invisible", !showColorButtons);
      $( "ul" ).empty();
      if (TextOsd.items.length > 0)
      {
          for (z = 0; z < TextOsd.items.length; z++)
          {
              var id = TextOsd.items[z].is_selected ? " id=\"selectedItem\"" : "";
              $( "ul" ).append("<li"+id+" class=\"item\">"+ TextOsd.items[z].content +"</li>")
          }
          $("div#content" ).toggleClass("invisible", false);
      }
      else
      {
          $("div#content" ).toggleClass("invisible", true);
      }
      this.oldOsdData = TextOsd;
    }
    //trigger refresh x milliseconds after rendering
    var x = setTimeout("OSDobj.refreshOsdData()", this.waitBeforeRefresh);
  }

}

 $(document).ready(function()
 {/*
    $("body").ajaxError(function(event, xhr, settings, exception)
    {
      $(this).text( "Triggered ajaxError handler." +
            xhr.responseText + "/" + xhr.status + "/" + settings.url );
    });*/
    var OSDobj = new VdrOsd();
    OSDobj.refreshOsdData();
});

    </script>
    <link rel="stylesheet" type="text/css" href="osd.css" />
    <title>HTML based VDR OSD (passive slave)</title>
  </head>
  <body>
  <div id="osd_container">
    <div class="invisible" id="header">&nbsp;</div>
    <div class="invisible" id="content">
    <ul type="none">
    </ul>
    </div><!-- closing content container -->
    <div class="invisible" id="color_buttons">
      <div id="red" class="first inactive">&nbsp;</div>
      <div id="green" class="second inactive">&nbsp;</div>
      <div id="yellow" class="third inactive">&nbsp;</div>
      <div id="blue" class="fourth inactive">&nbsp;</div>
      <br class="clear">
    </div><!-- closing color_buttons container -->
  </div><!-- closing osd_container -->
  </body>
</html>