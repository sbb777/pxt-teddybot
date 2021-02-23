// Auto-generated. Do not edit.



    //% color=50 weight=80
    //% icon="\uf1eb"
     //% deprecated=true
declare namespace teddybotmIR {

    /**
     * button pushed.
     */
    //% blockId=ir_received_left_event
    //% block="on |%btn| button pressed" shim=teddybotmIR::onPressEvent
     //% deprecated=true
    function onPressEvent(btn: RemoteButton, body: () => void): void;

    /**
     * initialises local variablesssss
     */
    //% blockId=ir_init
    //% block="connect ir receiver to %pin" shim=teddybotmIR::initIR
    function initIR(pin: Pins): void;
}



    //% color=50 weight=80
    //% icon="\uf1eb"
declare namespace teddybotmIRV2 {
}

// Auto-generated. Do not edit. Really.
