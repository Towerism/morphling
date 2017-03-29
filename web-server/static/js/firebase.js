// var database = firebase.database();

var gameId;
var statesRef = firebase.database().ref('states/' + gameId);

firebase.database().ref().on('child_added', function(snapshot) {
    // Do whatever
    snapshot.forEach(function(childSnapshot) {
      var childKey = childSnapshot.key;
      var childData = childSnapshot.val();
      //get board data
      //update board
    });
});

function updateBoard(snapshot) {
    "use strict";
    var board;
    board = ['X', '', '', '', '', '', '', '', ''];
    for (var i = 0; i < board.length; i++) {
      var cell = $('#gameboard td:eq('+i+')');
      //console.log(i, board[i], cell.text())
      cell.html(board[i])
		}
}
