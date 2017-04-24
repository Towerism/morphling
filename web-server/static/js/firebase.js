'use strict';

var gameId = $('#token').text();
console.log(gameId);

var pieceUrls = String($( '#piece_urls' ).data( "pieceurls" ))
            .replace(/(u)'([^']*)'/g, "\"$2\"")
            .replace(/'([^']*)'/g, "\"$1\"");
pieceUrls = jQuery.parseJSON(pieceUrls);
console.log(pieceUrls);
var pieces = []
var pieceCSS = {}
for (var k in pieceUrls){
  pieces.push(k);
  pieceCSS[k] = 'url(' + pieceUrls[k] + ') ' + 'no-repeat center center'
}
console.log(pieceCSS);
console.log(pieces);
var bg = String($( '#piece_urls' ).data( "bg" ));
var background = 'url(' + bg + ') ' + 'repeat';

var statesRef = firebase.database().ref('states/'+gameId.trim());
console.log('states/'+gameId.trim());
var count = 0;

firebase.database().ref('games/'+gameId.trim()).child('score').on('value', function(snapshot) {
  if (snapshot.val() !== null) {
    console.log(snapshot.val());
    var winner = snapshot.val();
    $( '#' + winner ).toggleClass('winner');
    statesRef.off();
    return true;
  }
});

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

statesRef.orderByKey().on('child_added', function(snapshot) {
    if(!background) background = getCSS('background','gb');
    $('#gameboard').css('background',background);

    updateBoard(snapshot);
});

function updateBoard(snapshot) {
    "use strict";
    count += 1;
    // console.log("update board");
    // console.log(count);
    // console.log(snapshot.key);
    var last = snapshot.val();
    // console.log(snapshot.key + ' ' + snapshot.val());
    var board = [];
    for (var key in last) {
      // console.log(key)
      //console.log(last[key]);
      var row = last[key].split(',')
      // console.log(row);
      for (var i in row) {
        for (var p in pieces) {
          if (row[i] === pieces[p]) {
            board.push(pieces[p]);
            break;
          }
        }
      }
    }

    console.log(board);
    for (var i = 0; i < board.length; i++) {
      var cell = $('#gameboard td:eq('+i+')');
      cell.css( 'background', pieceCSS[board[i]]);
		}
}
