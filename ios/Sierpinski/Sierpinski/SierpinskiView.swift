//
//  SierpinskiView.swift
//  Sierpinski
//
//  Created by Danny Sung on 09/02/2018.
//  Copyright © 2018 Sung Heroes LLC. All rights reserved.
//

import Foundation
import UIKit

@IBDesignable
class SierpinskiView: UIView {
    let opQueue: OperationQueue
    var sierpOp: SierpinskiCalculator?
    var timer: Timer?

    required init?(coder aDecoder: NSCoder) {
        let opQueue = OperationQueue()
        opQueue.qualityOfService = .background
        self.opQueue = opQueue
        
        super.init(coder: aDecoder)
        
        self.timer = Timer.scheduledTimer(withTimeInterval: 0.03, repeats: true, block: { (t) in
            self.setNeedsDisplay()
        })
    }
    deinit {
        self.timer?.invalidate()
    }

    override func layoutSubviews() {
        super.layoutSubviews()
        
        self.opQueue.cancelAllOperations()
        let op = SierpinskiCalculator(bounds: self.bounds.size,
                                      maxPoints: 50000)
        self.opQueue.addOperation(op)
        self.sierpOp = op
    }
    
    override func draw(_ rect: CGRect) {
        guard let gc = UIGraphicsGetCurrentContext() else { return }
        guard let verticies = self.sierpOp?.verticies else { return }
        guard let points = self.sierpOp?.points else { return }
        guard verticies.count > 0 else { return }
        
        // Draw Verticies
        gc.setFillColor(UIColor.red.cgColor)
        gc.setLineWidth(1)
        let radius: CGFloat = 5
        for v in verticies {
            let r = CGRect(x: v.x - radius, y: v.y-radius,
                           width: radius*2, height: radius*2)
            gc.addEllipse(in: r)
            gc.fillPath()
        }
        
        // Draw new point
        gc.setLineWidth(0)
//        gc.setFillColor(UIColor.white.cgColor)
        for point in points {
            let p = point.coordinate
            let r = CGRect(x: p.x, y: p.y, width: 1, height: 1)
            gc.setFillColor(point.color)
            gc.addRect(r)
            gc.fillPath()
        }
    }
}
