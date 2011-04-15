<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
    <head>
        <title>
            epsilon-minus-minus
        </title>
        <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
        <!--my CSS styling-->
        <style>
            html {
                    font-family:cursive;
                    margin:0;
                    padding:0;
                 }
            body {
                    color:white;
                    background:black;
                    margin:0;
                    padding:0;
                 }
            h1 {
                    text-align:center;
                    height:10%;
               }
            hr {
                    border:none;                    
              }
            button, select {
                    color:white;
                    background:black;
                    border-color:white;
                    }
            #inst_pages {
                    float:left;
                    width:66%;
                    height:40%;
                    overflow:auto;
                  }
            #wiki {
                    float:left;
                    width:33%;
                    height:40%;
                    overflow:auto;
                  }
            #nav {
                    clear:both;
                    height:10%;
                 }
            #core, #inst {
                    width:33%;
                    float:left;
                    }
             #def{
                    float:right;
                    height:40%;
                   }
                   #popup{
                       float:right;

                    }
                    #output {
                    height:40%;
                    overflow:auto;
                    }
                    
        </style>
        <script type="text/javascript" src="assets/js/jquery.js">
        </script>
        <!--my jQuery functions-->
        <script type="text/javascript">
        jQuery(function($){
            function recursive(target,execution){
                if(typeof(target) == 'object')
                    for(var i in target)
                        recursive(target[i], execution);
                else
                    execution(target);
            };
            $("div").ajaxStart(function(){
                $('#load').fadeIn('slow');
            });


            var page = 0;
            $(document).ready(function(){
                $('#def').slideUp('fast');
                $('#load').slideUp('fast');
            });

            $('#next').bind('click', function(e){//ADD PAGE CYCLING
                $('#inst_pages').load('test'+$page+'.txt');//LOW-LEVEL AJAX NEEDED
            });
            $('#prev').bind('click', function(e){//ADD PAGE CYCLING
                $('#inst_pages').load('test'+$page+'.txt');//LOW-LEVEL AJAX NEEDED
            });
            
            $('#def button').bind('click', function(e){//VERIFY SELECTOR
                var sender = $(this);
                $.ajax({
                    type: 'POST',
                    url: 'assets/async/get_wiki.php',
                    data: {
                        t: sender.val()
                    },
                    success: function(r){
                        $('#wiki').html(r);
                        $('#wiki').slideDown('slow');
                        $('#def').slideUp('slow');
                        $('#load').fadeOut('slow');
                    }
                });
            });
            
            $('#request').bind('click', function(e){
                var word = $('#w');
                var parity = $('#p');
                var type = $('#pyt');
                var error = $('#e');
                $.ajax({
                    type: 'POST',
                    url: 'assets/async/get_data.php',
                    data: {
                        w: word.val(),
                        p: parity.val(),
                        pyt: type.val(),
                        e: error.val()
                    },
                    success: function(r){
                        //var data = $.parseJSON(r);
                        alert('job submitted..');
                    }
                });
            });
            $('#display').bind('click', function(e){
                var url = 'assets/files/2-4-1-90.json';
                $.getJSON(url,function(j){
                    $.recursive(j, function(p){
                        $('#output').append(p + '<br />');
                    		    //weeeeeeeeeeeeeeeeeeeee
                    });
                });
            });
            $('#popup').bind('click', function(e){
                $('#wiki').slideUp('slow');
                $('#def').slideDown('slow');
            });
        });
    </script>
   
    </head>
    <body>
        
        <h1>epsilon-minus-minus<div id="load">
            loading..
        </div></h1>
        
        <hr>
        
        <!--instructional pages-->
        <div id="inst_pages">
            
                <h1>instructional</h1>
    welcome to the epsilon-minus-minus instructional!
    <br/>
    <br>

    epsilon-minus-minus is an open Source project that explores the properties of error-detection schemes in linear codes. Click previous and next to navigate the instructional.
            
       </div>
        <!--wikipedia pages-->
        <div id="wiki">
        </div>
        <div id="def">
                <button type="button" value="Parity Bit">parity bit
                </button>
                        <br>
                <button type="button" value="Panda">panda
                </button>
                <button type="button" value="Saltine">saltine
                </button>
            </div>



        <hr>
        
        <!--navigation toolbar-->
        <div id="nav">
            <div id="core">
                <button type="button" id="request">request data
                </button>
                <button type="button" id ="display">display results
                </button>
            </div>
            <!--instructionals-->
            <div id="inst">
                <button type="button" id="prev">previous
                </button>
                <button type="button" id="next">next
                </button>


            
                
            </div>
            <!--definitions-->
            <button type="button" id="popup">definitions
                </button>
        </div>
        
        <hr>
        
        <!--output page-->
        <div id ="output">
            
        </div>
        
        <!--input form-->
        <form id="input">
            word length:
            <select id="w">
                <?php
                for($i=2;$i<13;$i++)
                    echo '<option value="'.$i.'">'.$i.'</option>';
                ?>
            </select>
            <br>
            parity length:
            <select id="p">
                <?php
                for($i=2;$i<13;$i++)
                    echo '<option value="'.$i.'">'.$i.'</option>';
                ?>
            </select>
            <br>
            parity type:
            <select id="pyt">
                <option value="1" >Dense</option>
                <option value="2" >Low Density</option>
                <option value="4" >Random</option>
            </select>

           epsilon:
           <select id="e">
               <?php
               for($i=0.01;$i<=1.00;$i+=0.01)
                echo  '<option value="'.$i.'">'.$i.'</option>';
               ?>
           </select>
        </form>
    </body>
</html>
