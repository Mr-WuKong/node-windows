{
	"targets": [
		{
			"target_name": "windows",
			"sources": [ 
				"./src/windows.cc",
			],
			"link_settings":
			{
			},
			"include_dirs": [
				"<!(node -e \"require('nan')\")",
			]
		}
	] 
}