//
//  Sierpinski.swift
//  Sierpinski
//
//  Created by Danny Sung on 09/02/2018.
//  Copyright © 2018 Sung Heroes LLC. All rights reserved.
//

import UIKit

@IBDesignable
class SierpinskiSimpleView: UIView {
    var verticies: [CGPoint] = []
    var timer: Timer?
    var points: [CGPoint] = []
    let maxPoints = 10000

    required init?(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)
        self.timer = Timer.scheduledTimer(withTimeInterval: 0.003, repeats: true, block: { (t) in
            self.addPoint()
            self.setNeedsDisplay()
        })
    }
    deinit {
        self.timer?.invalidate()
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        self.setupVertices()
    }
    
    private func setupVertices() {
        var p: CGPoint
        var verticies: [CGPoint] = []
        
        p = CGPoint(x: self.bounds.width/2, y: 0)
        verticies.append(p)
        
        p = CGPoint(x: 0, y: self.bounds.height)
        verticies.append(p)
        
        p = CGPoint(x: self.bounds.width, y: self.bounds.height)
        verticies.append(p)
        
        self.verticies = verticies
    }

    // Only override draw() if you perform custom drawing.
    // An empty implementation adversely affects performance during animation.
    override func draw(_ rect: CGRect) {
        guard let gc = UIGraphicsGetCurrentContext() else { return }
        guard self.verticies.count > 0 else { return }
        
        // Draw Verticies
        gc.setFillColor(UIColor.red.cgColor)
        gc.setLineWidth(1)
        let radius: CGFloat = 5
        for v in self.verticies {
            let r = CGRect(x: v.x - radius, y: v.y-radius,
                           width: radius*2, height: radius*2)
            gc.addEllipse(in: r)
            gc.fillPath()
        }
        
        // Draw new point
        
        gc.setFillColor(UIColor.white.cgColor)
        gc.setLineWidth(0)
        for p in self.points {
            let r = CGRect(x: p.x, y: p.y, width: 1, height: 1)
            gc.addRect(r)
            gc.fillPath()
        }
    }
    
    var randomVertex: CGPoint {
        get {
            // Ref: https://learnappmaking.com/random-numbers-swift/
//            let n = Int.random(in: 0 ..< self.verticies.count)
            let n = arc4random_uniform( UInt32(self.verticies.count) )
            return self.verticies[Int(n)]
        }
    }
    
    func addPoint() {
        if self.points.count > self.maxPoints {
            self.points.removeFirst()
        }
        
        let lastPoint = self.points.last ?? .zero
        let newPoint = self.randomVertex
        let midPoint = lastPoint.midpoint(newPoint)

        self.points.append(midPoint)
    }
}

