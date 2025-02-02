export interface Dataset {
    id: number;
    title: string;
    description: string;
    selectedUnits: string[]
    "signals": [
        {
            "signalName": "signal1",
            "signalUnit": "string",
            "signalValues": [
                [1,1],
                [1,2],
                [1,3]
            ]
        }
    ]
}