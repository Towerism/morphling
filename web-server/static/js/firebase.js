'use strict';

var gameId = $('#token').text();
console.log(gameId);
var statesRef = firebase.database().ref('states/'+gameId.trim());
console.log('states/'+gameId.trim());
// console.log(statesRef.child(gameId).push().key);
var count = 0;
var settingsRef = firebase.database().ref('settings');
var piece_x, piece_o, background;

//Credits to http://stackoverflow.com/questions/10958869/jquery-get-css-properties-values-for-a-not-yet-applied-class
var getCSS = function (prop, fromClass) {
    var $inspector = $("<div>").css('display', 'none').addClass(fromClass);
    $("body").append($inspector); // add to DOM, in order to read the CSS property
    try {
        return $inspector.css(prop);
    } finally {
        $inspector.remove(); // and remove from DOM
    }
};

settingsRef.once('value').then(function(snapshot) {
  var background_url = snapshot.child("board/image").val();
  var piece_x_url =  snapshot.child("tokens/x/image").val();
  var piece_o_url =  snapshot.child("tokens/o/image").val();

  background = 'url(' + background_url + ') ' + 'repeat';
  piece_x = 'url(' + piece_x_url + ') ' + 'no-repeat center center';
  piece_o = 'url(' + piece_o_url + ') ' + 'no-repeat center center';
});

statesRef.orderByKey().on('child_added', function(snapshot) {
    if(!background) background = getCSS('background','gb');
    if(!piece_o) piece_o = getCSS('background','O');
    if(!piece_x) piece_x = getCSS('background','X');
    $('#gameboard').css('background',background);

    updateBoard(snapshot);
});

function updateBoard(snapshot) {
    "use strict";
    count += 1;
    console.log("update board");
    console.log(count);
    console.log(snapshot.key);
    var last = snapshot.val();
    // console.log(snapshot.key + ' ' + snapshot.val());
    var board = [];
    for (var key in last) {
      // console.log(key)
      //console.log(last[key]);
      var row = last[key].split(',')
      // console.log(row);
      for (var i in row) {
        if (row[i] === '_') board.push(' ');
        else if (row[i] === 'x') board.push('X');
        else if (row[i] === 'o') board.push('O');
        // console.log(row[i]);
      }
    }
    console.log(board);
    for (var i = 0; i < board.length; i++) {
      var cell = $('#gameboard td:eq('+i+')');
      console.log(i, board[i], cell.text());
      // cell.html(board[i]);
      // if (cell.hasClass( "X" )) cell.removeClass( "X" );
      // if (cell.hasClass( "O" )) cell.removeClass( "O" );

      if(board[i] === 'X') cell.css( 'background', piece_x );
      else if(board[i] === 'O') cell.css( 'background', piece_o );
		}
}
