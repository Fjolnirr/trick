exec(open("Modified_data/realtime.py").read())

print("This is trick-->", trick)
drg = trick.DRMongo("Fjorn")
drg.set_mongoDbUri("mongodb://rootuser:rootpass@10.41.74.77:27017")
drg.add_variable("dyn.cannon.pos[0]")
drg.add_variable("dyn.cannon.pos[1]")
drg.add_variable("dyn.cannon.vel[0]")
drg.add_variable("dyn.cannon.vel[1]")
drg.freq = trick.DR_Always
trick.add_data_record_group( drg , trick.DR_Buffer)

trick.exec_set_terminate_time(5.3)

