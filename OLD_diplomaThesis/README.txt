How to run it on Ubuntu 14.04 (Probably will work also on other version of Ubnuntu)
1) Install packages:
	sudo apt-get install texlive texlive-lang-czechslovak texlive-latex-extra cslatex
2) This thesis was written in TexWorks
	You can install TexWorks using: sudo apt-get install texworks
3) How to compile it using TexWorks?
	Edit in preferences command "PdfLaTeX" and add to the beggining of the parameters "--enable-write18"
4) Usefull tips to develop in the TexWorks: http://web.student.tuwien.ac.at/~e0325258/projects/c/texworks/manual-r39.pdf

How to run it on windows
1) Download full latex for windows https://www.tug.org/protext/
2) In TexStudio go to setttings.
3) Then click Commands and Change Latex Command to: "cslatex.exe -src -interaction=nonstopmode %.tex"
4) Then click Build and change Build & View to option "DVI->PS->PDF Chaing"
5) Then build and run by pressig F5