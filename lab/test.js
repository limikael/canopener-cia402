import {openSlcanBus, RemoteDevice} from "canopener";

let bus=await openSlcanBus({path: "/dev/ttyACM0", baudRate: 115200});
let dev=new RemoteDevice({bus, nodeId: 5});
let targetPosition=dev.entry(0x607A,0x00).setType("int32");
let actualPosition=dev.entry(0x6064,0x00).setType("int32");

let maxVel=dev.entry(0x6081,0x00).setType("int32");
let maxAccel=dev.entry(0x6083,0x00).setType("int32");
let maxDecel=dev.entry(0x6084,0x00).setType("int32");
let control=dev.entry(0x6040,0x00).setType("uint16");

await control.set(0x0f);
//await control.set(0x0);

await maxAccel.set(10000);
await maxDecel.set(10000);
await maxVel.set(16000);

let pos=process.argv[2];
if (!pos)
	throw new Error("No pos!");

await actualPosition.refresh();
console.log("Pos: "+actualPosition.get());

await targetPosition.set(pos);

while (actualPosition.get()!=targetPosition.get()) {
	await new Promise(r=>setTimeout(r,250));
	await actualPosition.refresh();
	console.log("Pos: "+actualPosition.get());
}

await bus.close();