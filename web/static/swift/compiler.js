var editor = CodeMirror.fromTextArea(document.getElementById('editor'), {
	mode : 'swift',
	gutters: ["CodeMirror-linenumbers", "error-gutter"],
	lineNumbers: true
});


function makeErrorMarker(err) {
  var marker = document.createElement("div");
  marker.style.color = "#822";
  marker.innerHTML = "●";
  marker.setAttribute('title', err.msg);
  return marker;
}

function reset() {
	editor.clearGutter('error-gutter');
	for(var i = 0; ; i++) {
		var line = editor.lineInfo(i);
		if(!line)
			break;
		editor.removeLineClass(i, 'background', 'error-line');
	}
	$('#ast').empty();
}
function create(nodeName, attrs, text) {
	var ret = document.createElement(nodeName);
	if(attrs) {
		if(typeof(attrs) == 'string') {
			ret.className = attrs;
		} else {
			for(var k in attrs) {
				ret.setAttribute(k, attrs[k]);
			}
		}
	}
	if(text) {
		ret.appendChild(document.createTextNode(text));
	}
	return ret;
}
function loadAST(node, container) {
	var div = create('DIV', 'node ' + node.node);
	var info = create('DIV', 'info');
	var name = create('SPAN', 'name', node.node);
	var children = create('DIV', 'children');
	info.appendChild(name);
	if(node.text) {
		info.appendChild(create('SPAN', 'text', node.text));
	}
	for(var k in node) {
		if(k == 'node' || k == 'text' || k == 'children')
			continue;
		var prop = create('DIV', 'prop');
		prop.appendChild(create('SPAN', 'prop-key', k));
		prop.appendChild(create('SPAN', '', ':'));
		prop.appendChild(create('SPAN', 'prop-value', node[k]));
		info.appendChild(prop);
	}
	div.appendChild(info);
	for(var i = 0; i < node.children.length; i++) {
		loadAST(node.children[i], children);
	}
	div.appendChild(children);
	container.appendChild(div);
	return div;
};

$('#compile').click(function() {
 	var code = editor.getValue();
 	reset();
 	$.ajax({
 		type : 'POST',
 		url : '/swift/compiler/ast',
 		data : code,
 		success : function(data) {
 			$('#errors').empty();
 			var levels = ['Fatal', 'Error', 'Warning', 'Note'];
 			for(var i = 0; i < data.errors.length; i++) {
 				var err = data.errors[i];
 				var html = '<tr><td>' + [i+1, levels[err.level], err.line + ',' + err.column, err.msg].join('</td><td>') + '</td></tr>';
 				$('#errors').append(html);
 				editor.setGutterMarker(err.line - 1, 'error-gutter', makeErrorMarker(err));
 				editor.addLineClass(err.line - 1, 'background', 'error-line');
 			}
 			//remove previous gutter
 			if(data.ast) {
 				loadAST(data.ast, document.getElementById('ast'));

 			}
 		}
 	});
});
$('#compile').click();