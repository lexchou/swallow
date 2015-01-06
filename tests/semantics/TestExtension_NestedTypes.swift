extension Int {
    enum Kind {
        case Negative, Zero, Positive
    }
    var kind: Kind {
        switch self {
        case 0:
            return .Zero
        case let x where x > 0:
            return .Positive
        default:
            return .Negative
        }
    }
}
func printIntegerKinds(numbers: [Int]) {
    for number in numbers {
        switch number.kind {
        case .Negative:
            print("- ")
        case .Zero:
            print("0 ")
        case .Positive:
            print("+ ")
        }
    }
    print("\n")
}
printIntegerKinds([3, 19, -27, 0, -6, 0, 7])