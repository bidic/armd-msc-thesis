package pl.com.mcg.client;

import java.util.Collection;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.DecoratorPanel;
import com.google.gwt.user.client.ui.HorizontalPanel;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.TextBox;
import com.google.gwt.user.client.ui.VerticalPanel;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class ClientSample implements EntryPoint {

	public void onModuleLoad() {
		RootPanel.get().add(createSecurityTestPanel());
		RootPanel.get().add(createEntityTestPanel());

		final Label resultLabel = new Label("Session bean test");

		Button b = new Button("stateful - get time");
		b.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(final ClickEvent event) {
				GwtStatefulSBSServiceAsync.Util.getInstance().getTime(new AsyncCallback<String>() {

					@Override
					public void onSuccess(String result) {
						resultLabel.setText(result);
					}

					@Override
					public void onFailure(Throwable caught) {
						resultLabel.setText(caught.getMessage());
					}
				});
			}
		});

		Button b2 = new Button("stateful - test add");
		b2.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(final ClickEvent event) {
				GwtStatefulSBSServiceAsync.Util.getInstance().addItem("test", new AsyncCallback<Void>() {

					@Override
					public void onSuccess(Void result) {
						resultLabel.setText("element added");
					}

					@Override
					public void onFailure(Throwable caught) {
						resultLabel.setText(caught.getMessage());
					}
				});
			}
		});

		Button b3 = new Button("stateful - test add");
		b3.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(final ClickEvent event) {
				GwtStatefulSBSServiceAsync.Util.getInstance().getItems(new AsyncCallback<Collection>() {

					@Override
					public void onSuccess(Collection result) {
						resultLabel.setText("curr size: " + result.size());
					}

					@Override
					public void onFailure(Throwable caught) {
						resultLabel.setText(caught.getMessage());
					}
				});
			}
		});

		Button b4 = new Button("stateless - get time");
		b4.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(final ClickEvent event) {
				GwtStatelessSBSServiceAsync.Util.getInstance().getTime(new AsyncCallback<String>() {

					@Override
					public void onSuccess(String result) {
						resultLabel.setText(result);
					}

					@Override
					public void onFailure(Throwable caught) {
						resultLabel.setText(caught.getMessage());
					}
				});
			}
		});

		Button b5 = new Button("stateless - test add");
		b5.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(final ClickEvent event) {
				GwtStatelessSBSServiceAsync.Util.getInstance().addItem("test", new AsyncCallback<Void>() {

					@Override
					public void onSuccess(Void result) {
						resultLabel.setText("element added");
					}

					@Override
					public void onFailure(Throwable caught) {
						resultLabel.setText(caught.getMessage());
					}
				});
			}
		});

		Button b6 = new Button("stateless - test add");
		b6.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(final ClickEvent event) {
				GwtStatelessSBSServiceAsync.Util.getInstance().getItems(new AsyncCallback<Collection>() {

					@Override
					public void onSuccess(Collection result) {
						resultLabel.setText("curr size: " + result.size());
					}

					@Override
					public void onFailure(Throwable caught) {
						resultLabel.setText(caught.getMessage());
					}
				});
			}
		});

		HorizontalPanel hPanel = new HorizontalPanel();
		hPanel.add(b);
		hPanel.add(b2);
		hPanel.add(b3);
		hPanel.add(b4);
		hPanel.add(b5);
		hPanel.add(b6);

		DecoratorPanel panel = new DecoratorPanel();
		VerticalPanel vPanel = new VerticalPanel();
		vPanel.add(resultLabel);
		vPanel.add(hPanel);
		panel.add(vPanel);

		RootPanel.get().add(panel);

	}

	/**
	 * @return
	 */
	private DecoratorPanel createEntityTestPanel() {
		final Label resultLabel = new Label("Test entity bean - gf");
		final TextBox firstnameField = new TextBox();
		firstnameField.setValue("Radosław");
		final TextBox lastnameField = new TextBox();
		lastnameField.setValue("Moczygęba");
		Button button = new Button("Dodaj użytkownika");
		button.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				TestServiceAsync.Util.getInstance().createCustomer(firstnameField.getValue(), lastnameField.getValue(),
						new AsyncCallback<String>() {

							@Override
							public void onSuccess(String result) {
								resultLabel.setText(result);

							}

							@Override
							public void onFailure(Throwable caught) {
								resultLabel.setText(caught.getMessage());
							}
						});
			}
		});

		HorizontalPanel hPanel = new HorizontalPanel();
		hPanel.add(firstnameField);
		hPanel.add(lastnameField);
		hPanel.add(button);

		DecoratorPanel panel = new DecoratorPanel();
		VerticalPanel vPanel = new VerticalPanel();
		vPanel.add(resultLabel);
		vPanel.add(hPanel);
		panel.add(vPanel);
		return panel;
	}

	private DecoratorPanel createSecurityTestPanel() {
		HorizontalPanel securityPanel = new HorizontalPanel();

		final Label resultLabel = new Label("Test bezpieczeństwa");
		final TextBox usernameField = new TextBox();
		usernameField.setValue("a");
		final TextBox passwordField = new TextBox();
		passwordField.setValue("A1234567");

		final Button loginButton = new Button("Login");
		loginButton.addClickHandler(new ClickHandler() {

			@Override
			public void onClick(ClickEvent event) {
				TestServiceAsync.Util.getInstance().login(usernameField.getValue(), passwordField.getValue(), new AsyncCallback<String>() {
					@Override
					public void onSuccess(String result) {
						resultLabel.setText(result);
					}

					@Override
					public void onFailure(Throwable caught) {
						resultLabel.setText(caught.getMessage());
					}
				});
			}
		});
		final Button logoutButton = new Button("Logout");
		logoutButton.addClickHandler(new ClickHandler() {

			@Override
			public void onClick(ClickEvent event) {
				TestServiceAsync.Util.getInstance().logout(new AsyncCallback<String>() {
					@Override
					public void onSuccess(String result) {
						resultLabel.setText(result);
					}

					@Override
					public void onFailure(Throwable caught) {
						resultLabel.setText(caught.getMessage());
					}
				});
			}
		});

		final Button welcomeButton = new Button("Say hello");
		welcomeButton.addClickHandler(new ClickHandler() {

			@Override
			public void onClick(ClickEvent event) {
				TestServiceAsync.Util.getInstance().sayHello(new AsyncCallback<String>() {
					@Override
					public void onSuccess(String result) {
						resultLabel.setText("Server: " + result);
					}

					@Override
					public void onFailure(Throwable caught) {
						resultLabel.setText(caught.getMessage());
					}
				});
			}
		});

		securityPanel.add(usernameField);
		securityPanel.add(passwordField);
		securityPanel.add(loginButton);
		securityPanel.add(logoutButton);
		securityPanel.add(welcomeButton);
		VerticalPanel vPanel = new VerticalPanel();
		vPanel.add(resultLabel);
		vPanel.add(securityPanel);

		DecoratorPanel panel = new DecoratorPanel();
		panel.add(vPanel);
		return panel;
	}

	// /**
	// * The message displayed to the user when the server cannot be reached or
	// * returns an error.
	// */
	// private static final String SERVER_ERROR = "An error occurred while "
	// + "attempting to contact the server. Please check your network "
	// + "connection and try again.";
	//
	// /**
	// * Create a remote service proxy to talk to the server-side Greeting service.
	// */
	// private final GreetingServiceAsync greetingService = GWT.create(GreetingService.class);
	//
	// private final Messages messages = GWT.create(Messages.class);
	//
	// /**
	// * This is the entry point method.
	// */
	// public void onModuleLoad() {
	// final Button sendButton = new Button( messages.sendButton() );
	// final TextBox nameField = new TextBox();
	// nameField.setText( messages.nameField() );
	// final Label errorLabel = new Label();
	//
	// // We can add style names to widgets
	// sendButton.addStyleName("sendButton");
	//
	// // Add the nameField and sendButton to the RootPanel
	// // Use RootPanel.get() to get the entire body element
	// RootPanel.get("nameFieldContainer").add(nameField);
	// RootPanel.get("sendButtonContainer").add(sendButton);
	// RootPanel.get("errorLabelContainer").add(errorLabel);
	//
	// // Focus the cursor on the name field when the app loads
	// nameField.setFocus(true);
	// nameField.selectAll();
	//
	// // Create the popup dialog box
	// final DialogBox dialogBox = new DialogBox();
	// dialogBox.setText("Remote Procedure Call");
	// dialogBox.setAnimationEnabled(true);
	// final Button closeButton = new Button("Close");
	// // We can set the id of a widget by accessing its Element
	// closeButton.getElement().setId("closeButton");
	// final Label textToServerLabel = new Label();
	// final HTML serverResponseLabel = new HTML();
	// VerticalPanel dialogVPanel = new VerticalPanel();
	// dialogVPanel.addStyleName("dialogVPanel");
	// dialogVPanel.add(new HTML("<b>Sending name to the server:</b>"));
	// dialogVPanel.add(textToServerLabel);
	// dialogVPanel.add(new HTML("<br><b>Server replies:</b>"));
	// dialogVPanel.add(serverResponseLabel);
	// dialogVPanel.setHorizontalAlignment(VerticalPanel.ALIGN_RIGHT);
	// dialogVPanel.add(closeButton);
	// dialogBox.setWidget(dialogVPanel);
	//
	// // Add a handler to close the DialogBox
	// closeButton.addClickHandler(new ClickHandler() {
	// public void onClick(ClickEvent event) {
	// dialogBox.hide();
	// sendButton.setEnabled(true);
	// sendButton.setFocus(true);
	// }
	// });
	//
	// // Create a handler for the sendButton and nameField
	// class MyHandler implements ClickHandler, KeyUpHandler {
	// /**
	// * Fired when the user clicks on the sendButton.
	// */
	// public void onClick(ClickEvent event) {
	// sendNameToServer();
	// }
	//
	// /**
	// * Fired when the user types in the nameField.
	// */
	// public void onKeyUp(KeyUpEvent event) {
	// if (event.getNativeKeyCode() == KeyCodes.KEY_ENTER) {
	// sendNameToServer();
	// }
	// }
	//
	// /**
	// * Send the name from the nameField to the server and wait for a response.
	// */
	// private void sendNameToServer() {
	// // First, we validate the input.
	// errorLabel.setText("");
	// String textToServer = nameField.getText();
	// if (!FieldVerifier.isValidName(textToServer)) {
	// errorLabel.setText("Please enter at least four characters");
	// return;
	// }
	//
	// // Then, we send the input to the server.
	// sendButton.setEnabled(false);
	// textToServerLabel.setText(textToServer);
	// serverResponseLabel.setText("");
	// greetingService.greetServer(textToServer, new AsyncCallback<String>() {
	// public void onFailure(Throwable caught) {
	// // Show the RPC error message to the user
	// dialogBox.setText("Remote Procedure Call - Failure");
	// serverResponseLabel.addStyleName("serverResponseLabelError");
	// serverResponseLabel.setHTML(SERVER_ERROR);
	// dialogBox.center();
	// closeButton.setFocus(true);
	// }
	//
	// public void onSuccess(String result) {
	// dialogBox.setText("Remote Procedure Call");
	// serverResponseLabel.removeStyleName("serverResponseLabelError");
	// serverResponseLabel.setHTML(result);
	// dialogBox.center();
	// closeButton.setFocus(true);
	// }
	// });
	// }
	// }
	//
	// // Add a handler to send the name to the server
	// MyHandler handler = new MyHandler();
	// sendButton.addClickHandler(handler);
	// nameField.addKeyUpHandler(handler);
	// }
}
