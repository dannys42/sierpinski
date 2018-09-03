//
//  SierpinskiCalculator.swift
//  Sierpinski
//
//  Created by Danny Sung on 09/02/2018.
//  Copyright © 2018 Sung Heroes LLC. All rights reserved.
//

import Foundation
import UIKit

struct Point {
    let coordinate: CGPoint
    let color: CGColor
}

class SierpinskiCalculator: Operation {
    private let maxPoints: Int
    private let bounds: CGSize
    public private(set) var points: [Point] = []
    public private(set) var verticies: [CGPoint] = []
    private var pointColor: UIColor = .white
    
    init(bounds: CGSize, maxPoints: Int) {
        self.bounds = bounds
        self.maxPoints = maxPoints
        
        super.init()
        self.setupVertices()
    }
    
//    public func getPoints() -> [Point] {
//        let points = self.points
//        return points
//    }
    
    override func main() {
        while !self.isCancelled {
            self.addPoint()
        }
    }
}

fileprivate extension SierpinskiCalculator {
    func setupVertices() {
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

    var randomVertex: CGPoint {
        get {
            // Ref: https://learnappmaking.com/random-numbers-swift/
            //            let n = Int.random(in: 0 ..< self.verticies.count)
            let n = arc4random_uniform( UInt32(self.verticies.count) )
            return self.verticies[Int(n)]
        }
    }
    
    func addPoint() {
        guard self.verticies.count > 0 else { sleep(1); return }
        
        if self.points.count > self.maxPoints {
            self.points.removeFirst()
        }
        
        let lastLocation = self.points.last?.coordinate ?? .zero
        let newLocation = self.randomVertex
        let midPointLocation = lastLocation.midpoint(newLocation)
        
        let newPoint = Point(coordinate: midPointLocation, color: self.pointColor.cgColor)
        
        self.points.append(newPoint)
    }
}
