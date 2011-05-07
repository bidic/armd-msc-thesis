/// Typ wyliczeniowy reprezentujacy rozmiar obrazu 
/// jaki ma zostac pobrany z kamery
public enum ImageSize {
	/// Obrazek maly rozmiar 160x100
	Small = 0,
	/// Obrazek sredni rozmiar 320x200
	Medium = 1,
	/// Obrazek duzy rozmiar 640x400
	High = 2
}
///	Typ wyliczeniowy reprezentujacy tryb pracy silnikow
public enum EnginesDirection {
	/// Tryb postoju
	Stop = 0,
	/// Tryb jazdy do przodu
	Straight = 1,
	/// Tryb jazdy wstecz
	Back = 2
}
/// Typ wyliczeniowy reprezentujacy typy pomiaru
public enum SensorType {
	/// Pomiar napiecia na baterii
	Battery = 0,
	/// Pomiar temperatury
	Thermometer = 1,
	/// Pomiar przyspieszenia
	Accelerometer = 2,
	/// Pomiar odleglosci
	Rangefinder = 3,
	/// Pomiar indukcji pola magnetycznego
	Magnetometer = 4,
	/// Pomiar kata obrotu
	Gyroscope = 5
}
/// Typ wyliczeniowy reprezentujacy polecenia obslugi 
/// trybu rekonstrukcji sciezki
public enum TrackReconstructionCommand {
	/// Rozpoczecie nagrywania sciezki
	StartRecordingTrack = 0,
	/// Koniec nagrywania sciezki
	StopRecordingTrack = 1,
	/// Usuniecie nagranej sciezki
	EraseRecordedTrack = 2,
	/// Rozpoczecie rekonstrukcji sciezki
	StartTrackReconstruction = 3,
	/// Przerwanie rekonstrukcji sciezki
	StopTrackReconstruction = 4,
	/// Przeslanie informacji o sciezce
	SendTrackData = 5,
}
/// Typ wyliczeniowy reprezentujacy polecenia 
/// obslugi trybu autonomicznego
public enum AutonomousModeCommand {
	/// Wylaczenie trybu autonomicznego
	DisableAutonomousMode = 0,
	/// Tryb autonomiczny - ksztalty
	EnableShapeDetection = 1,
	/// Tryb autonomiczny - twarz
	EnableFaceDetection = 2,
	/// Przeslanie wspolrzednych obiektu
	SendDetectionResult = 3,
	/// Przeslanie zbinaryzowanego obrazu
	SendBinarizedImage = 4
}
/// Typ wyliczeniowy reprezentujacy 
/// dostepne rodzaje polecen sterujacych
public enum RequestType {
	/// Test polaczenia
	Welcome = 0, 
	/// Sterowanie silnikami
	Engines = 1,
	/// Sterowanie serwomechanizmem
	Servomechanism = 2,
	/// Sterowanie kamera
	Camera = 3,
	/// Sterowanie czujnikami
	Sensors = 4,
	/// Sterowanie rekonstrukcja trasy
	TrackReconstruction = 5,
	/// Sterowanie trybem autonomicznym
	AutonomousMode = 6
}