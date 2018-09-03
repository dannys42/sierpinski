//
//  CGPointExtension.swift
//  Sierpinski
//
//  Created by Danny Sung on 09/02/2018.
//  Copyright © 2018 Sung Heroes LLC. All rights reserved.
//

import Foundation
import UIKit

public extension CGPoint {
    func midpoint(_ p: CGPoint) -> CGPoint {
        return CGPoint(x: (self.x + p.x)/2,
                       y: (self.y + p.y)/2)
    }
}
