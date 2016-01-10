function saveTextAsFile(fileName)
{
	var fileNameToSaveAs = fileName;
	var textToWrite;

	 textToWrite = $("#graphe").text();

	var textFileAsBlob = new Blob([textToWrite], {type:'text/plain'});
	

	var downloadLink = document.createElement("a");
	downloadLink.download = fileNameToSaveAs;
	downloadLink.innerHTML = "Download File";
	if (window.webkitURL != null)
	{
		// Chrome allows the link to be clicked
		// without actually adding it to the DOM.
		downloadLink.href = window.webkitURL.createObjectURL(textFileAsBlob);
	}
	else
	{
		// Firefox requires the link to be added to the DOM
		// before it can be clicked.
		downloadLink.href = window.URL.createObjectURL(textFileAsBlob);
		downloadLink.onclick = destroyClickedElement;
		downloadLink.style.display = "none";
		document.body.appendChild(downloadLink);
	}

	downloadLink.click();
}

function destroyClickedElement(event)
{
	document.body.removeChild(event.target);
}

$(document).ready(function()
{
	var compt=1;
	$('#gen').on("mousedown",function(){


		var nbSommet=Math.floor(Math.random() * 6) + 4  ;
		var nbArc=Math.floor(Math.random() * ((nbSommet-1)*nbSommet -4)) + 4  ;
		var arc=new Array();

		var txt=nbSommet+"\n";
		txt+=nbArc+"\n";

		for(var i=0;i<nbArc;i++)
		{
			var nb1;
			var nb2;
			do{
				nb1=Math.floor(Math.random() * (nbSommet-1)) + 1;
				nb2=Math.floor(Math.random() * (nbSommet-1))+ 1;
			}while(nb1==nb2 || txt.indexOf(nb1+","+nb2+',') != -1);


			var poids=Math.floor(Math.random() * 40) -20;
			txt+=nb1+","+nb2+","+poids+"\n";

			arc.push(nb1+""+nb2);
		}

		$("#graphe").html(txt);
		saveTextAsFile("graphe_"+compt+".txt");
		compt++;
	});

	$("label input[type=file]").change(function(event)
	{
		var file = this.files[0];
		fileName=file.name.substr(0,file.name.lastIndexOf('.'));
		if (file) 
		{
			var reader = new FileReader();
			reader.readAsText(file);
			var txt = $(this).parent().siblings('textarea').eq(0);
			reader.onload = function(e) 
			{
				$(txt).val(e.target.result);
				disable();
			};
		}
	});


	
});