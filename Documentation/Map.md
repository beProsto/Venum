## Function, copied from "p5.js" project. Used to map a value in one range, to another.
### Map(Value, Start1, Stop1, Start2, Stop2)

- Takes in a Value in range from Start1 to Stop1, and then "Maps" it into a value in range from Start2 to Stop2.
  - Uses this calculation: ((Value - Start1) / (Stop1 - Start1)) * (Stop2 - Start2) + Start2
- Returns the "Mapped" value.
