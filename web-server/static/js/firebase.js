'use strict';

var gameId = String($( '#gameId' ).data( "tk" ));
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
    var i = 0;
    for (var key in last) {
      // console.log(key)
      //console.log(last[key]);
      var row = last[key].split(',')
      // console.log(row);
      for (var r in row) {
        board.push(row[r]);
        var cell = $('#gameboard td:eq('+i+')');
        if (row[r] in pieceCSS) cell.css( 'background', pieceCSS[row[r]] );
        i += 1;
      }
    }
    console.log(board);
}
