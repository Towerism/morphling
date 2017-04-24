'use strict';

var gameId = $('#token').text();
console.log(gameId);

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

statesRef.orderByKey().on('child_added', function(snapshot) {
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
