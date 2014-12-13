class Address {
    var buildingName: String?
    var buildingNumber: String?
    var street: String?
    func buildingIdentifier() -> String? {
        if buildingName != nil {
            return buildingName
        } else if buildingNumber != nil {
            return buildingNumber
        } else {
            return nil
        }
    }
}
class Room
{
}
class Residence {
    var rooms = [Room]()
    var numberOfRooms: Int {
        return rooms.count
    }
    subscript(i: Int) -> Room {
        get {
            return rooms[i]
        }
        set {
            rooms[i] = newValue
        }
    }
    func printNumberOfRooms() {
        println("The number of rooms is \(numberOfRooms)")
    }
    var address: Address?
}

class Person {
    var residence: Residence?
}

let john = Person()
let someAddress = Address()
someAddress.buildingNumber = "29"
someAddress.buildingName = "The"
someAddress.street = "Acacia Road"
john.residence = Residence()
john.residence?.address = someAddress

//if let buildingIdentifier = john.residence?.address?.buildingIdentifier() {
//    println(buildingIdentifier);
//}

if let beginsWithThe =
    john.residence?.address?.buildingIdentifier()?.hasPrefix("The") {
        if beginsWithThe {
            println("John's building identifier begins with \"The\".")
        } else {
            println("John's building identifier does not begin with \"The\".")
        }
}

// prints "Unable to retrieve the address."

/*
(bind_optional_expr type='Optional<String>'
  (call_expr type='Optional<Optional<String>>'
    (dot_expr field='buildingIdentifier' type='null'
      (bind_optional_expr type='Address'
        (dot_expr field='address' type='Optional<Address>'
          (bind_optional_expr type='Residence'
            (dot_expr field='residence' type='Optional<Residence>'
              (pattern_named 'john' type='Person'))))))
    (paren_expr type='null')))
   */
