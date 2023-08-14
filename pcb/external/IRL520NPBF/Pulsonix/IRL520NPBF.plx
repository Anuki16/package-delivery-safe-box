PULSONIX_LIBRARY_ASCII "SamacSys ECAD Model"
//934732/1114575/2.50/3/4/MOSFET N-Channel

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "c192_h128"
		(holeDiam 1.28)
		(padShape (layerNumRef 1) (padShapeType Ellipse)  (shapeWidth 1.92) (shapeHeight 1.92))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 1.92) (shapeHeight 1.92))
	)
	(padStyleDef "s192_h128"
		(holeDiam 1.28)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 1.92) (shapeHeight 1.92))
		(padShape (layerNumRef 16) (padShapeType Rect)  (shapeWidth 1.92) (shapeHeight 1.92))
	)
	(textStyleDef "Normal"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 1.27)
			(strokeWidth 0.127)
		)
	)
	(patternDef "TO254P469X1042X1967-3P" (originalName "TO254P469X1042X1967-3P")
		(multiLayer
			(pad (padNum 1) (padStyleRef s192_h128) (pt 0, 0) (rotation 90))
			(pad (padNum 2) (padStyleRef c192_h128) (pt 2.54, 0) (rotation 90))
			(pad (padNum 3) (padStyleRef c192_h128) (pt 5.08, 0) (rotation 90))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 0, 0) (textStyleRef "Normal") (isVisible True))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -2.98 3.445) (pt 8.06 3.445) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 8.06 3.445) (pt 8.06 -1.745) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 8.06 -1.745) (pt -2.98 -1.745) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -2.98 -1.745) (pt -2.98 3.445) (width 0.05))
		)
		(layerContents (layerNumRef 28)
			(line (pt -2.73 3.195) (pt 7.81 3.195) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 7.81 3.195) (pt 7.81 -1.495) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 7.81 -1.495) (pt -2.73 -1.495) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -2.73 -1.495) (pt -2.73 3.195) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -2.73 1.925) (pt -1.46 3.195) (width 0.025))
		)
		(layerContents (layerNumRef 18)
			(line (pt 7.81 -1.495) (pt 7.81 3.195) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt 7.81 3.195) (pt -2.73 3.195) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -2.73 3.195) (pt -2.73 0) (width 0.2))
		)
	)
	(symbolDef "IRL520NPBF" (originalName "IRL520NPBF")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 100 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 0 mils -45 mils) (rotation 0]) (justify "UpperLeft") (textStyleRef "Normal"))
		))
		(pin (pinNum 2) (pt 300 mils 400 mils) (rotation 270) (pinLength 100 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 305 mils 400 mils) (rotation 90]) (justify "UpperLeft") (textStyleRef "Normal"))
		))
		(pin (pinNum 3) (pt 300 mils -200 mils) (rotation 90) (pinLength 100 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 305 mils -200 mils) (rotation 90]) (justify "LowerLeft") (textStyleRef "Normal"))
		))
		(line (pt 300 mils 100 mils) (pt 300 mils -100 mils) (width 6 mils))
		(line (pt 300 mils 200 mils) (pt 300 mils 300 mils) (width 6 mils))
		(line (pt 100 mils 0 mils) (pt 200 mils 0 mils) (width 6 mils))
		(line (pt 200 mils 0 mils) (pt 200 mils 200 mils) (width 6 mils))
		(line (pt 300 mils 100 mils) (pt 230 mils 100 mils) (width 6 mils))
		(line (pt 300 mils 200 mils) (pt 230 mils 200 mils) (width 6 mils))
		(line (pt 230 mils 0 mils) (pt 300 mils 0 mils) (width 6 mils))
		(line (pt 230 mils 220 mils) (pt 230 mils 180 mils) (width 6 mils))
		(line (pt 230 mils -20 mils) (pt 230 mils 20 mils) (width 6 mils))
		(line (pt 230 mils 80 mils) (pt 230 mils 120 mils) (width 6 mils))
		(arc (pt 250 mils 100 mils) (radius 150 mils) (startAngle 0) (sweepAngle 360) (width 10  mils))
		(poly (pt 230 mils 100 mils) (pt 270 mils 120 mils) (pt 270 mils 80 mils) (pt 230 mils 100 mils) (width 10  mils))
		(attr "RefDes" "RefDes" (pt 450 mils 150 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))
		(attr "Type" "Type" (pt 450 mils 50 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))

	)
	(compDef "IRL520NPBF" (originalName "IRL520NPBF") (compHeader (numPins 3) (numParts 1) (refDesPrefix Q)
		)
		(compPin "1" (pinName "G") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "2" (pinName "D") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "3" (pinName "S") (partNum 1) (symPinNum 3) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "IRL520NPBF"))
		(attachedPattern (patternNum 1) (patternName "TO254P469X1042X1967-3P")
			(numPads 3)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
				(padNum 3) (compPinRef "3")
			)
		)
		(attr "Manufacturer_Name" "Infineon")
		(attr "Manufacturer_Part_Number" "IRL520NPBF")
		(attr "Mouser Part Number" "942-IRL520NPBF")
		(attr "Mouser Price/Stock" "https://www.mouser.co.uk/ProductDetail/Infineon-Technologies/IRL520NPBF?qs=9%252BKlkBgLFf3eZ5ppEpIaKQ%3D%3D")
		(attr "Arrow Part Number" "IRL520NPBF")
		(attr "Arrow Price/Stock" "https://www.arrow.com/en/products/irl520npbf/infineon-technologies-ag?region=nac")
		(attr "Description" "Infineon IRL520NPBF N-channel MOSFET, 10 A, 100 V LogicFET, 3-Pin TO-220AB")
		(attr "<Hyperlink>" "https://www.infineon.com/dgdl/irl520npbf.pdf?fileId=5546d462533600a40153565f9b62255b")
		(attr "<Component Height>" "4.69")
		(attr "<STEP Filename>" "IRL520NPBF.stp")
		(attr "<STEP Offsets>" "X=0;Y=0;Z=0")
		(attr "<STEP Rotation>" "X=0;Y=0;Z=0")
	)

)
