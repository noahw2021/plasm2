# plasm2

Upcoming version of plasm, in extreme alpha. Some things may not work. Many things are untested. All projects are stored in this repository.


## source control
main branch is now protected, pull requests required!

## non-explicit documentation
I plan on fully documenting this instruction set, however some important things that are not super clear in the code are laid out here.

### system timer interrupt
This system timer interrupt fires every fixed amount of nanoseconds, by default 10,000 NS (100 fires per MS, 100,000 fires per S), and this time cannot be changed by the system. This calls interrupt '0x00'.

### interrupt system
The interrupt system is very simple, and has constraints on what an interrupt can do, and how multiple interrupts are handled.

#### interrupt queue
The interrupt queue system logic is computed every clock, however not if the system is currently inside of an interrupt. It will wait for this interrupt to return before firing the next one. Thus, it is very important that only trusted system code is executed as an interrupt, as otherwise the system can very easily hang. This may be updated in the future, altough this instruction set is designed to be fully backwards compatible, so it is safe to assume that this scheme of interrupts being unable to interrupt interrupts is true, unless a flag is set.

The system will pick the newest interrupt to fire after the current interrupt returns.

The system will walk through the entire interrupt queue before firing a system timer / clock interrupt.

#### interrupt stacking
Interrupts cannot stack, as in an interrupt cannot call another interrupt.
