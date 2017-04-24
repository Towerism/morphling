var pieceUrls = String($( '#piece_urls' ).data( "pieceurls" ))
            .replace(/(u)'([^']*)'/g, "\"$2\"")
            .replace(/'([^']*)'/g, "\"$1\"");
pieceUrls = jQuery.parseJSON(pieceUrls);
console.log(pieceUrls);
var pieces = []
var pieceCSS = {}
for (var k in pieceUrls) {
  pieces.push(k);
  pieceCSS[k] = 'url(' + pieceUrls[k] + ') ' + 'no-repeat center center'
}
console.log(pieceCSS);
console.log(pieces);
var bg = String($( '#piece_urls' ).data( "bg" ));
var background = 'url(' + bg + ') ' + 'repeat';
$('#gameboard').css('background',background);
