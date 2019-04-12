///////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                           //
//				+ Projet de INF1900                                                          //
//				+ Groupe 4951	                                                             //
//				+ Nom du Robot :                              		                         //
//				+ Membres d'Équipe : - Xavier Brazeau			                             //       
//                                 	 - Yan Joliot	                                         //
//                                 	 - Hany Bassi		                                     //
//                                 	 - Simon Zhang 1839892                                   //
//				+ Date de dernière modification : Dimanche 14 avril 2019	                 //
//                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                           //
//				+ Le dossier du projet se retrouve sous tp/Projet/							 //
//					+ Celui-ci contient: 													 //
//							+ la librairie sous lib_dir/									 //
//							+ les fichiers executables des deux robot sous exec_dir/		 //
//																							 //
//				+ Afin de creer l'executable, vous pouvez directement depuis la racine		 //
//				  du repertoire faire appel a un "make clean", "make", et "make install"	 //
//																							 //
//				+ Il existe deux modes des executables: 									 //
//					+ le mode "DEBUG" permet d'activer toutes les fonctions de debogage		 //
//					  qui utilisent le UART pour transmettre des messages et donc permet 	 //
//					  a l'utilisateur de faire une instrumentation du code. On peut 		 //
//					  l'activer en settant la variable "DEBUG_CMD" dans le fichier 			 //
//					  "debugging.mk" du dossier "exec_dir" actif a "DEBUG". 				 //
//					+ le mode "NDEBUG" permet au preprocesseur d'eliminer toutes les 		 //
//					  fonctions de debogage. Ainsi, c'est un mode synonyme a un mode 		 //
//					  "RELEASE". Puisque le code ne fait pas appel a <assert.h>, nous		 //
//					  pouvons simplement rien mettre comme argument de la variable			 //
//					  "DEBUG_CMD" dans le fichier "debugging.mk", sinon on utiliserait		 //
//					  "NDEBUG" comme valeur de la variable.									 //
//																							 //
//				+ Il peut exister plusieurs fichiers aussi divers a la racine du repertoire: //
//					+ un fichier de configuration pour un build bot comme Travis-CI			 //
//					+ un script permettant de changer de mode 								 //
//					+ des liens symboliques vers les dossiers de travail					 //
//					+ des fichiers liee au lintage du milieu de travail inutilises			 //
//                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                           //
//				+ "Merci Jerome" - Simon Zhang												 //
//				+																			 //
//				+																			 //
//				+																			 //
//                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////