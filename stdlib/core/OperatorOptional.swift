

func ??<T>(optional: T?, defaultValue: @autoclosure () -> T) -> T {
    return T()

}

func ??<T>(optional: T?, defaultValue: @autoclosure () -> T?) -> T? {
    return nil;
}
