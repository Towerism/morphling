'use strict';

var gameId = $('#token').text();
console.log(gameId);
var statesRef = firebase.database().ref('states/'+gameId.trim());
console.log('states/'+gameId.trim());
// console.log(statesRef.child(gameId).push().key);
var count = 0;
statesRef.orderByKey().on('child_added', function(snapshot) {
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
      console.log(i, board[i], cell.text())
      cell.html(board[i])
		}
}
