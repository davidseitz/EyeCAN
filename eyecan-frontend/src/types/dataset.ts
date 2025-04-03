export interface Dataset {
    id: string;
    title: string;
    description: string;
    selectedSignalValues: string[];
    signals: Signal[];
}

interface Signal {
    signalName: string;
    signalUnit: string;
    signalValues: [number, number][];
}
