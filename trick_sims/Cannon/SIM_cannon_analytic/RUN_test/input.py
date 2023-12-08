exec(open("Modified_data/realtime.py").read())

print("This is trick-->", trick)
drg = trick.DRMongo("Fjorn")
drg.add_variable("dyn.cannon.pos[0]")
drg.add_variable("dyn.cannon.pos[1]")
drg.freq = trick.DR_Always
trick.add_data_record_group( drg , trick.DR_Buffer)


trick.exec_set_terminate_time(5.3)

