/** For checking if a URL is valid */
let urlRegex = [%bs.re "/^((?:https?:\/\/)?[^./]+(?:\.[^./]+)+(?:\/.*)?)$/"];

let scriptUrl = "document.URL";

/** TODO: Need to find some good logic for this,
Right now it's here to demonstrate erro validation */
let scriptCompany = "document.h1";

/** Idea: gather all headers, find which one is the largest -- this one usually indicates the role for which one is applying  */
/** NOTE: MAKE SURE TO DEFINE FUNCTIONS AS VAR, SO THAT THE CODE CAN REDECLARE THE FUNCTIONS EVERY TIME THE PAGE IS RENDERED */
let scriptPosition = "  var getAreaAndText = el => ({ fontSize: parseFloat(window.getComputedStyle(el, null).getPropertyValue('font-size')), text: el.innerText });
                        var isVisible = el => el.offsetWidth > 0 || el.offsetHeight > 0;
                        var retrieveHeader = header => Array.prototype.slice.call(document.getElementsByTagName(header)).filter(isVisible).map(getAreaAndText);

                        [...retrieveHeader('h1'), ...retrieveHeader('h2'), ...retrieveHeader('h3'),
                        ...retrieveHeader('h4'), ...retrieveHeader('h5'), ...retrieveHeader('h6')]
                        .filter(Boolean).sort((a, b) => b.fontSize - a.fontSize)[0].text";

/** Error checking with comfy optional types  */
let validateNonNull = x =>
  switch (x) {
  | None => failwith("Value is null")
  | Some(x) => x
  };

let toStringProcess = x => Js.String.make(x);

let checkValidUrl = x => {
  let stringUrl = x |> Js.String.make;
  Js.Re.test(stringUrl, urlRegex) ? x : failwith("Invalid URL");
};

let validateUrl = x => x |> validateNonNull |> checkValidUrl;
