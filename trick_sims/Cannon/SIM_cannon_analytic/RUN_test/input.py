exec(open("Modified_data/cannon.dr").read())
exec(open("Modified_data/realtime.py").read())


drg = trick.DRAscii("Fjorn")
drg.add_variable("dyn.cannon.pos[0]")
drg.freq = trick.DR_Always
trick.add_data_record_group( drg , trick.DR_Buffer)


trick.exec_set_terminate_time(5.2)

